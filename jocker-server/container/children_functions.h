#ifndef JOCKER_SERVER_CHILDREN_FUNCTIONS_H
#define JOCKER_SERVER_CHILDREN_FUNCTIONS_H

#include "container_options.h"
#include "ns_options/ns_conf_repository.h"
#include "ns_types/ns.h"
#include "common.h"
#include <memory>

struct child_argument {
    child_argument(std::vector<std::pair<ns_type, std::string>> &other_ns_to_create, const container_options &other_opts, ns_conf_repository other_repo): opts(other_opts), ns_to_create(other_ns_to_create), repo(std::move(other_repo)) {}
    std::vector<std::pair<ns_type, std::string>> ns_to_create;
    container_options opts;
    ns_conf_repository repo;
};

void child_init_ns(std::vector<std::pair<ns_type, std::string>> &ns_to_create, ns_conf_repository &repo){
    // The next part of code inits just created namespaces

    // Get namespaces
    pid_t pid = getpid();
    std::shared_ptr<ns> new_ns;
    std::vector<std::shared_ptr<ns>> new_namespaces;
    for (const auto &entry: ns_to_create) {
        int fd = get_ns_handle(entry.first, pid);
        new_namespaces.emplace_back(create_namespace_entry(entry.first, entry.second, fd, pid));
    }

    // Setup process
    for (const auto &ns: new_namespaces){
        ns->init_internal(repo);
    }
}

static int child_function(void *arg){
    auto parent_info = static_cast<child_argument *>(arg);

    child_init_ns(parent_info->ns_to_create, parent_info->repo);

    // Start the binary
    // TODO: close fds
    auto progname = strdup(parent_info->opts.bin_path.c_str());
    auto args_ptr = createCharPtrArray(parent_info->opts.bin_arguments);
    //sleep(3);
    auto error = execvp(progname, args_ptr.get()); // TODO: handle the errors
    perror("execvp");
    return 0;
}


#endif //JOCKER_SERVER_CHILDREN_FUNCTIONS_H
