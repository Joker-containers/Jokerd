#ifndef JOCKER_SERVER_CONTAINER_H
#define JOCKER_SERVER_CONTAINER_H

#include "cgroup.h"
#include "ns_group.h"
#include "container_options.h"
#include <string>


// A class to contain info about container: resource controllers settings, namespaces configurations etc.

class container {
public:
    container(const container_options &opts);
private:
    // Can we rely on info about container contained in this instance?
    // If some external impact on configs was spotted... Well that's user's problems know, so we invalidate this object... Or no?
    bool valid;

    // Container name
    std::string m_cname;

    // Cgroup of this container
    cgroup m_cgroup;

    // Namespaces of this container
    ns_group m_namespaces;
};


#endif //JOCKER_SERVER_CONTAINER_H
