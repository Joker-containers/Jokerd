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

static int              /* Start function for cloned child */
child_function(void *arg)
{
    struct utsname uts;

    /* Change hostname in UTS namespace of child. */

    if (sethostname(static_cast<char *>(arg), strlen(static_cast<char *>(arg))) == -1)
        err(EXIT_FAILURE, "sethostname");

    /* Retrieve and display hostname. */

    if (uname(&uts) == -1)
        err(EXIT_FAILURE, "uname");
    printf("uts.nodename in child:  %s\n", uts.nodename);

    /* Keep the namespace open for a while, by sleeping.
       This allows some experimentation--for example, another
       process might join the namespace. */

    sleep(5);

    // TODO: make wrapper function for this stuff
    char **new_argv = new char *[2];
    char *progname_copy = new char [strlen(static_cast<char *>(arg)) + 1];
    strcpy(progname_copy, static_cast<char *>(arg));
    new_argv[0] = progname_copy;
    new_argv[1] = nullptr;

    auto error = execvp(static_cast<char *>(arg), new_argv); // TODO: handle the errors
    perror("execvp");
    return 0;
}

container::container(const container_options &opts, d_resources &daemon) {
    m_cname = opts.container_name;
    // mount -t proc proc /proc In the new fs
    int new_ns_flags = 0;
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
            new_ns_flags |= it->second;
        }
    }

    {
        pid_t           pid;
        struct utsname  uts;

        char child_stack[CHILD_STACK_SIZE];
        void            *stack;         /* Start of stack buffer */
        void            *stackTop;      /* End of stack buffer */

        stack = static_cast<void *>(&child_stack);
        stackTop = static_cast<void *>(&child_stack[CHILD_STACK_SIZE]); /* Assume stack grows downward */

        /* Create child that has its own namespaces;
           child commences execution in childFunc(). */
        void *test = strdup("test"); // Test
        pid = clone(child_function, &stackTop, new_ns_flags | SIGCHLD, test); // Last argument is just for testing now

        if (pid == -1) err(EXIT_FAILURE, "clone"); // TODO: handle this

        if (waitpid(pid, NULL, 0) == -1)    /* Wait for child */
            err(EXIT_FAILURE, "waitpid"); // TODO: handle this
        printf("child has terminated\n");
    }
}