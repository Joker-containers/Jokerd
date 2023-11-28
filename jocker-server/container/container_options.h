#ifndef JOCKER_SERVER_CONTAINER_OPTIONS_H
#define JOCKER_SERVER_CONTAINER_OPTIONS_H


#include <string>
#include "cgroup_options.h"
#include "ns_options/ns_options.h"


class container_options {
public:
    ns_options namespace_options;
    cgroup_options cgroup_options;
    std::vector<std::string> bin_arguments;
    std::string container_name;

    std::string bin_path;
};


#endif //JOCKER_SERVER_CONTAINER_OPTIONS_H
