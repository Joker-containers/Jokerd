#ifndef JOCKER_SERVER_CHILDREN_FUNCTIONS_H
#define JOCKER_SERVER_CHILDREN_FUNCTIONS_H

#include "container_options.h"
#include "ns_options/ns_conf_repository.h"
#include "ns_types/ns.h"
#include "common.h"
#include <memory>

struct child_argument {
    explicit child_argument(const container_options &other_opts, const auto &namespaces): opts(other_opts), namespaces(namespaces){}
    container_options opts;
    ns_group namespaces;
};

void child_init_ns(const ns_group &namespaces){
    // The next part of code inits just created namespaces

    // Setup process
    const auto &namespace_collection = namespaces.get_namespaces();
    const auto &namespaces_mask = namespaces.get_ns_mask();
    for (size_t i = 0; i < NS_TYPES_NUM; ++i){
        if (namespaces_mask[i]){
            namespace_collection[i]->internal_setup_ns();
        }
    }

    for (size_t i = 0; i < NS_TYPES_NUM; ++i){
        if (namespaces_mask[i]){
            namespace_collection[i]->init_internal();
        }
    }
}

void redirect_to_logs(int fd){
    syscall_wrapper(dup2, "dup2", fd, STDOUT_FILENO);
    syscall_wrapper(dup2, "dup2", fd, STDERR_FILENO);
}

static int child_function(void *arg){
    auto parent_info = static_cast<child_argument *>(arg);
    sleep(1);
    child_init_ns(parent_info->namespaces);

    // Start the binary
    // TODO: close fds
    auto progname = strdup(parent_info->opts.bin_path.c_str());
    auto args = parent_info->opts.bin_arguments;
    args.insert(args.cbegin(), progname);
    auto args_ptr = createCharPtrArray(args);
    //sleep(3);
    redirect_to_logs(parent_info->opts.output_fd.fd);
    auto error = execvp(progname, args_ptr.get()); // TODO: handle the errors
    perror("execvp");
    return 0;
}


#endif //JOCKER_SERVER_CHILDREN_FUNCTIONS_H
