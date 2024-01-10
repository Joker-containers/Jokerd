#ifndef JOCKER_SERVER_CONTAINER_OPTIONS_H
#define JOCKER_SERVER_CONTAINER_OPTIONS_H

#include "common.h"
#include "ns_group.h"
#include <string>
#include <utility>

/**
 * Contains options to create a container
 */
class container_options {
public:
    container_options(const container_options &other) = default;

    container_options(ns_group namespaces, std::vector<std::string> bin_arguments, std::string bin_path,
                      std::string container_name, std::string cgroup_name, int fd) :
                                                            namespaces(std::move(namespaces)),
                                                            bin_path(std::move(bin_path)),
                                                            bin_arguments(std::move(bin_arguments)),
                                                            container_name(std::move(container_name)),
                                                            cgroup_name(std::move(cgroup_name)),
                                                            output_fd(fd) {}

    ns_group namespaces;
    std::string bin_path;
    std::vector<std::string> bin_arguments;
    int output_fd = -1;
    std::string container_name;
    std::string cgroup_name;
};


#endif //JOCKER_SERVER_CONTAINER_OPTIONS_H
