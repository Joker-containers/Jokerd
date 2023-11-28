#ifndef JOCKER_SERVER_CONTAINER_H
#define JOCKER_SERVER_CONTAINER_H

#include "cgroup.h"
#include "ns_group.h"
#include "container_options.h"
#include "ns_pool.h"
#include "ns_options/ns_conf_repository.h"
#include <string>
#include <sys/utsname.h>
#include <err.h>
#include <csignal>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>

struct d_resources{
    ns_pool& d_ns_pool;
    ns_conf_repository& conf_repo;
    // Cgroup pool to be added?
};

extern const std::unordered_map<ns_type, int> ns_clone_flag;

/* A class to contain info about container: resource controllers settings, namespaces configurations etc. */
class container {
public:
    container(const container_options &opts, d_resources &daemon);

    pid_t perform_clone(int new_ns_flags, const container_options &opts, std::vector<std::pair<ns_type, std::string>> &ns_to_create, ns_conf_repository &repo);

private:
    void init_namespaces(const ns_conf_repository &opts);

    // Can we rely on info about container contained in this instance?
    // If some external impact on configs was spotted... Well that's user's problems know, so we invalidate this object... Or no?
    bool valid{};

    // Container name
    std::string m_cname;

    // Cgroup of this container
    cgroup m_cgroup;

    // Namespaces of this container
    ns_group m_namespaces;
};

struct child_argument {
    child_argument(std::vector<std::pair<ns_type, std::string>> &other_ns_to_create, ns_group &other_ns, const container_options &other_opts, ns_conf_repository repo);
    std::vector<std::pair<ns_type, std::string>> ns_to_create;
    ns_group namespaces;
    container_options opts;
    ns_conf_repository repo;
};


#endif //JOCKER_SERVER_CONTAINER_H
