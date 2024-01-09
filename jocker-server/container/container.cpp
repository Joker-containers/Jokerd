#include "children_functions.h"
#include "container.h"
#include "common.h"
#include <sched.h>          /* Definition of CLONE_* constants */
#include <utility>

constexpr size_t CHILD_STACK_SIZE = 1024*1024; // Megabyte /* Stack size for cloned child */

int container::prepare_flags() {
    int new_ns_flags;
    const auto &namespaces = _namespaces.get_namespaces();
    const auto &mask = _namespaces.get_ns_mask();
    for (size_t i = 0; i < NS_TYPES_NUM; ++i){
        // Explicitly casted i to emphasize that index corresponds to the namespace type
        const auto ns_t = static_cast<ns_type>(i);
        if (mask[ns_t] && !namespaces[ns_t]->is_active()){
            auto it = NS_CLONE_FLAG.find(ns_t);
            if (it == NS_CLONE_FLAG.end()){
                throw std::runtime_error("Requested to create a namespace of an unsupported type!"); // TODO: add more description
            }
            new_ns_flags |= it->second;
        }
    }
    return new_ns_flags;
}

pid_t container::perform_clone(int new_ns_flags, const container_options &opts, std::vector<std::pair<ns_type, std::string>> &ns_to_create, ns_conf_repository &repo){
    pid_t           pid;

    char child_stack[CHILD_STACK_SIZE];
    void            *stackTop;      /* End of stack buffer */

    stackTop = static_cast<void *>(&child_stack[CHILD_STACK_SIZE]); /* Assume stack grows downward */

    /* Create child that has its own namespaces;
       child commences execution in childFunc(). */

    auto *arg = new child_argument(ns_to_create, opts, repo); // TODO: make smart pointer, as memory leak on error happens!
    pid = syscall_wrapper(clone, "clone", child_function, &stackTop, new_ns_flags | SIGCHLD, static_cast<void *>(arg));
    delete arg;

    return pid;
}

void container::init_namespaces(const ns_conf_repository &opts,
                                const std::vector<std::pair<ns_type, std::string>>&ns_to_create,
                                d_resources &daemon_resources,
                                pid_t pid) {
    // Update info in namespaces, which existed already
    const auto namespaces_to_update = _namespaces.get_namespaces();
    for (const auto &entry: namespaces_to_update){
        // There are nullptr entries for non-existing namespaces
        if (entry.get()){
            entry->add_pid(pid);
        }
    }

    // Create handles for new namespaces
    std::shared_ptr<ns> new_ns;
    for (const auto &entry: ns_to_create) {
        int fd = get_ns_handle(entry.first, pid);
        new_ns = std::move(create_namespace_entry(entry.first, entry.second, fd, pid));
        daemon_resources.d_ns_pool.declare_ns(entry.first, new_ns);
        _namespaces.add_ns(entry.first, std::move(new_ns));
    }

    auto &ns_collection = _namespaces.get_namespaces();
    auto &ns_collection_mask = _namespaces.get_ns_mask();
    for (size_t i = 0; i < ns_collection.size(); ++i){
        if (ns_collection_mask[i]){
            ns_collection[i]->external_setup_ns(opts);
        }
    }

    for (size_t i = 0; i < ns_collection.size(); ++i){
        if (ns_collection_mask[i]){
            ns_collection[i]->init_external(opts);
        }
    }
}

container::container(container_options opts): _cname(std::move(opts.container_name)){ //TODO
    valid = true; // TODO
    int new_ns_flags = 0;

    // Separate already existing namespaces and namespaces to create; set flags
    auto ns_to_create = prepare_flags();

    pid_t pid = perform_clone(new_ns_flags, opts, ns_to_create, daemon_resources.conf_repo);

    // New namespaces are created now; we should initialize them.
    init_namespaces(daemon_resources.conf_repo, ns_to_create, daemon_resources, pid);
}
