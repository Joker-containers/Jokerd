#ifndef JOCKER_SERVER_CONTAINER_H
#define JOCKER_SERVER_CONTAINER_H

#include "cgroup.h"
#include "ns_group.h"
#include "container_options.h"
#include "ns_pool.h"
#include <string>
#include <sys/utsname.h>
#include <err.h>
#include <csignal>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>

struct d_resources{
    ns_pool& d_ns_pool;
    // Cgroup pool to be added?
};

extern const std::unordered_map<ns_type, int> ns_clone_flag;

/* A class to contain info about container: resource controllers settings, namespaces configurations etc. */
class container {
public:
    container(const container_options &opts, d_resources &daemon);
private:
    void init_namespaces(const ns_options &opts);

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


#endif //JOCKER_SERVER_CONTAINER_H
