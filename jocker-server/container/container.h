#ifndef JOCKER_SERVER_CONTAINER_H
#define JOCKER_SERVER_CONTAINER_H

#include "cgroup.h"
#include "ns_group.h"
#include "container_options.h"
#include "ns_pool.h"
#include "ns_options/ns_conf_repository.h"
#include "cgroup_manager.h"
#include <string>
#include <sys/utsname.h>
#include <err.h>
#include <csignal>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>

struct d_resources{
    cgroup_manager& cgr_manager;
};

extern const std::unordered_map<ns_type, int> NS_CLONE_FLAG;

/* A class to contain info about container: resource controllers settings, namespaces configurations etc. */
class container {
public:
    container(container_options opts, d_resources &resources);

    static pid_t perform_clone(int new_ns_flags, const container_options &opts, const ns_group& namespaces);

private:
    void init_namespaces(pid_t pid);

    int prepare_flags();

    // Can we rely on info about container contained in this instance?
    // If some external impact on configs was spotted... Well that's user's problems know, so we invalidate this object... Or no?
    bool valid{};

    // Container name
    std::string _cname;

    // Cgroup of this container
    cgroup _cgroup;

    // Namespaces of this container
    ns_group _namespaces;
};


#endif //JOCKER_SERVER_CONTAINER_H
