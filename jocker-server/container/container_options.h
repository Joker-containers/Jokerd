#ifndef JOCKER_SERVER_CONTAINER_OPTIONS_H
#define JOCKER_SERVER_CONTAINER_OPTIONS_H


#include <string>
#include <utility>
#include "cgroup_options.h"
#include "ns_options/ns_options.h"


class container_options {
public:
    container_options(const container_options &other) = default;

    container_options(ns_options &other_opts, cgroup_options cgroup_opts,
                      std::vector<std::string> &bin_arguments, std::string &bin_path, std::string container_name): namespace_options(other_opts),
                                                                                             cgroup_options_m(cgroup_opts),
                                                                                             bin_path(bin_path),
                                                                                             bin_arguments(bin_arguments),
                                                                                             container_name(std::move(container_name)){}

    ns_options namespace_options;
    cgroup_options cgroup_options_m;
    std::string bin_path;
    std::vector<std::string> bin_arguments;
    std::string container_name;
};


#endif //JOCKER_SERVER_CONTAINER_OPTIONS_H
