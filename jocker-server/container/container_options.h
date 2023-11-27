#ifndef JOCKER_SERVER_CONTAINER_OPTIONS_H
#define JOCKER_SERVER_CONTAINER_OPTIONS_H


#include <string>
#include "cgroup_options.h"
#include "ns_options.h"


class container_options {
public:
    ns_options my_ns_opts;
    cgroup_options my_cgroup_opts;
    std::string container_name;
};


#endif //JOCKER_SERVER_CONTAINER_OPTIONS_H
