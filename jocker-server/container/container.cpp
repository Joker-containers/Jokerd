#include "children_functions.h"
#include "container.h"
#include "common.h"
#include <sched.h>          /* Definition of CLONE_* constants */
#include <utility>

constexpr size_t CHILD_STACK_SIZE = 1024*1024; // Megabyte /* Stack size for cloned child */

int container::prepare_flags() {
    int new_ns_flags = 0;
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

pid_t container::perform_clone(int new_ns_flags, const container_options &opts, const ns_group& namespaces) {
    pid_t           pid;

    char child_stack[CHILD_STACK_SIZE];
    void            *stackTop;      /* End of stack buffer */

    stackTop = static_cast<void *>(&child_stack[CHILD_STACK_SIZE]); /* Assume stack grows downward */

    /* Create child that has its own namespaces;
       child commences execution in childFunc(). */

    auto *arg = new child_argument(opts, namespaces); // TODO: make smart pointer, as memory leak on error happens!
    pid = syscall_wrapper(clone, "clone", child_function, &stackTop, new_ns_flags | SIGCHLD, static_cast<void *>(arg));
    delete arg;

    return pid;
}

void container::init_namespaces(pid_t pid) {
    // Update info in namespaces
    const auto &namespaces = _namespaces.get_namespaces();
    const auto &ns_collection_mask = _namespaces.get_ns_mask();
    for (const auto &entry: namespaces){
        // There are nullptr entries for non-existing namespaces
        if (entry.get()){
            entry->add_pid(pid);
        }
    }

    // Create handles for new namespaces
    for (size_t i = 0; i < NS_TYPES_NUM; ++i) {
        const auto &entry = namespaces[i];
        if (ns_collection_mask[i] && !entry->is_active()){
            int fd = get_ns_handle(static_cast<ns_type>(entry->get_type()), pid); // TODO make this in a better way
            entry->set_fd(fd);
        }
    }

    for (size_t i = 0; i < namespaces.size(); ++i){
        if (ns_collection_mask[i]){
            namespaces[i]->external_setup_ns();
        }
    }

    for (size_t i = 0; i < namespaces.size(); ++i){
        if (ns_collection_mask[i]){
            namespaces[i]->init_external();
        }
    }

    for (size_t i = 0; i < NS_TYPES_NUM; ++i){
        const auto &entry = namespaces[i];
        if (ns_collection_mask[i] && !entry->is_active()){
            entry->set_active();
        }
    }
}

container::container(container_options opts): _cname(opts.container_name),
                                                _cgroup_name(opts.cgroup_name),
                                                _namespaces(std::move(opts.namespaces)){ //TODO
    valid = true; // TODO
    int new_ns_flags = 0;

    // Separate already existing namespaces and namespaces to create; set flags
    new_ns_flags = prepare_flags();

    pid_t pid = perform_clone(new_ns_flags, opts, _namespaces);

    // Add container process to cgroup
    cgroup_manager::add_child(_cgroup_name, pid);

    // New namespaces are created now; we should initialize them.
    init_namespaces(pid);
}
