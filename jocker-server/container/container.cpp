#include "container.h"

constexpr size_t CHILD_STACK_SIZE = 1024*1024; // Megabyte /* Stack size for cloned child */

const std::unordered_map<ns_type, int> ns_clone_flag = {
        {IPC,     CLONE_NEWIPC},
        {NETWORK, CLONE_NEWNET},
        {MOUNT,   CLONE_NEWNS},
        {PID,     CLONE_NEWPID},
        // {TIME, CLONE_NEWTIME}, // It seems that it is not supported on my Linux version
        {USER, CLONE_NEWUSER},
        {UTS, CLONE_NEWUTS},
};

const std::unordered_map<ns_type, std::string> ns_shortcut = {
        {IPC, "ipc"},
        {NETWORK, "net"},
        {MOUNT, "mnt"},
        {PID, "pid"},
        {USER, "user"},
        {TIME, "time"},
        {UTS, "uts"}
};

static std::string get_ns_handle_path(ns_type tp, pid_t pid){
    std::stringstream path;
    path << "/proc/" << pid << "/ns/" << ns_shortcut.at(tp);
    return path.str();
}

static std::shared_ptr<ns> create_namespace(ns_type tp, const std::string &ns_name, int fd, pid_t pid){
    std::shared_ptr<ns> new_ns;
    switch (tp) {
        case IPC:
            new_ns = std::make_shared<ipc_ns>(ns_name, fd, pid);
            break;
        case NETWORK:
            new_ns = std::make_shared<net_ns>(ns_name, fd, pid);
            break;
        case MOUNT:
            new_ns = std::make_shared<mnt_ns>(ns_name, fd, pid);
            break;
        case PID:
            new_ns = std::make_shared<pid_ns>(ns_name, fd, pid);
            break;
        case TIME:
            new_ns = std::make_shared<time_ns>(ns_name, fd, pid);
            break;
        case USER:
            new_ns = std::make_shared<user_ns>(ns_name, fd, pid);
            break;
        case UTS:
            new_ns = std::make_shared<uts_ns>(ns_name, fd, pid);
            break;
        default:
            throw std::runtime_error("Unsupported namespace type!");
    }
    return new_ns;
}

// TODO: transfer this to utilities
void deleteCharPtrArray(char **av) {
    if (av) {
        for (size_t i = 0; av[i]; ++i) {
            delete[] av[i];
        }
        delete[] av;
    }
}

std::unique_ptr<char *[], void(*)(char **)> createCharPtrArray(const std::vector<std::string> &tokens) {
    std::unique_ptr<char *[], void(*)(char **)> ptr(new char *[tokens.size() + 1], deleteCharPtrArray);
    ptr[tokens.size()] = nullptr;
    for (size_t i = 0; i < tokens.size(); ++i) {
        ptr[i] = new char[tokens[i].length() + 1];
        std::strcpy(ptr[i], tokens[i].c_str());
    }
    return ptr;
}

//

static int child_function(void *opts){
    auto converted_opts = static_cast<container_options *>(opts);

//    std::shared_ptr<ns> new_ns;
//    for (const auto &entry: ns_to_create) {
//        int fd = get_ns_handle(entry.first, pid);
//        new_ns = std::move(create_namespace(entry.first, entry.second, fd, pid));
//        m_namespaces.add_ns(entry.first, new_ns);
//    }
//    init_namespaces(opts.my_ns_opts);

    auto progname = strdup(converted_opts->bin_path.c_str());
    auto args_ptr = createCharPtrArray(converted_opts->bin_arguments);
    auto error = execvp(progname, args_ptr.get()); // TODO: handle the errors
    perror("execvp");
    return 0;
}

static int get_ns_handle(ns_type tp, pid_t pid){
    auto path = get_ns_handle_path(tp, pid);
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1){
        throw std::runtime_error("Failed to open namespace handle!");
    }
    return fd;
}

pid_t perform_clone(int new_ns_flags, const container_options &opts){
    pid_t           pid;

    char child_stack[CHILD_STACK_SIZE];
    void            *stack;         /* Start of stack buffer */
    void            *stackTop;      /* End of stack buffer */

    stack = static_cast<void *>(&child_stack);
    stackTop = static_cast<void *>(&child_stack[CHILD_STACK_SIZE]); /* Assume stack grows downward */

    /* Create child that has its own namespaces;
       child commences execution in childFunc(). */
    void *test = strdup("test"); // Test
    auto opts_copy = new container_options(opts);
    pid = clone(child_function, &stackTop, new_ns_flags | SIGCHLD, static_cast<void *>(opts_copy)); // Last argument is just for testing now

    if (pid == -1) err(EXIT_FAILURE, "clone"); // TODO: handle this

//    if (waitpid(pid, NULL, 0) == -1)    /* Wait for child */
//        err(EXIT_FAILURE, "waitpid"); // TODO: handle this
//    printf("child has terminated\n");
    return pid;
}

void container::init_namespaces(const ns_options &opts) {
    auto ns_collection = m_namespaces.get_required_ns();
    auto ns_collection_mask = m_namespaces.get_ns_mask();
    for (size_t i = 0; i < ns_collection.size(); ++i){
        if (ns_collection_mask[i]){
            ns_collection[i]->setup_ns(opts);
        }
    }

    for (size_t i = 0; i < ns_collection.size(); ++i){
        if (ns_collection_mask[i]){
            ns_collection[i]->init_external();
        }
    }
}

container::container(const container_options &opts, d_resources &daemon) {
    m_cname = opts.container_name;
    m_namespaces = {};
    valid = true; // TODO
    // mount -t proc proc /proc In the new fs
    int new_ns_flags = 0;
    std::vector<std::pair<ns_type, std::string>> ns_to_create;
    const auto &ns_names = opts.my_ns_opts.get_required_ns();
    const auto &ns_mask = opts.my_ns_opts.get_ns_mask();

    assert(ns_names.size() == NS_TYPES_NUM);

    for (size_t i = 0; i < ns_names.size(); ++i){
        // Explicitly casted i to emphasize that index corresponds to the namespace type
        if (ns_mask[static_cast<ns_type>(i)] && !daemon.d_ns_pool.exists_ns(static_cast<ns_type>(i), ns_names[static_cast<ns_type>(i)])){
            auto it = ns_clone_flag.find(static_cast<const ns_type>(i));
            if (it == ns_clone_flag.end()){
                throw std::runtime_error("Requested to create a namespace of an unsupported type!"); // TODO: add more description
            }
            ns_to_create.emplace_back(static_cast<ns_type>(i), ns_names[static_cast<ns_type>(i)]);
            new_ns_flags |= it->second;
        } else if (ns_mask[static_cast<ns_type>(i)] && daemon.d_ns_pool.exists_ns(static_cast<ns_type>(i), ns_names[static_cast<ns_type>(i)])){
            m_namespaces.add_ns(static_cast<ns_type>(i), daemon.d_ns_pool.get_ns(static_cast<ns_type>(i), ns_names[static_cast<ns_type>(i)]));
        }
        else {

        }
    }

    pid_t pid = perform_clone(new_ns_flags, opts);
    // New namespaces are created now; we should create corresponding objects in the runtime.
    std::shared_ptr<ns> new_ns;
    for (const auto &entry: ns_to_create) {
        int fd = get_ns_handle(entry.first, pid);
        new_ns = std::move(create_namespace(entry.first, entry.second, fd, pid));
        daemon.d_ns_pool.register_ns(entry.first, new_ns);
        m_namespaces.add_ns(entry.first, new_ns);
    }
    init_namespaces(opts.my_ns_opts);
}
