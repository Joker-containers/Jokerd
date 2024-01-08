//
// Created by Nazar Demchuk on 28.11.2023.
//

#include <fstream>
#include <iostream>
#include <boost/property_tree/ini_parser.hpp>
#include "parser.h"

Parser::Parser(const std::string &file_name) : file_name(file_name) {}

container_options Parser::parse_options(const std::string& binary_name) {
    std::ifstream config(file_name);

    if(!config.is_open()) {
        throw std::ifstream::failure("Unable to open config file: " + file_name);
    }

    auto [namespace_map, cgroup_map, debug_map] = parse_ini(config);

    config.close();

    ns_options namespace_options;
    std::string bin_path = binary_name;
    std::vector<std::string> bin_arguments;
    std::string container_name = binary_name;

    for (size_t i = 0; i != NS_TYPES_NUM; i++) {
        if (namespace_map.contains(ns_type_strings[(int)i])) {
            namespace_options._ns_collection[i] = namespace_map[ns_type_strings[(int)i]];
            namespace_options._entry_valid[i] = true;
        }
        namespace_options._entry_valid[i] = false;
    }

    // TODO: ADD FUCKING CGROUPS PARSING, LIUBOMYR!!!

    container_options cont_options(namespace_options, bin_arguments, bin_path, container_name, -1);

    return cont_options;
}

std::tuple<std::unordered_map<std::string, std::string>,
        std::unordered_map<std::string, std::string>,
        std::unordered_map<std::string, std::string>> Parser::parse_ini(const std::ifstream& file) {
    std::stringstream ss;
    ss << file.rdbuf();

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_ini(ss, pt);
    } catch (const boost::property_tree::ini_parser_error& e) {
        throw std::runtime_error("Error parsing INI file");
    }

    std::unordered_map<std::string, std::string> namespace_map, cgroup_map, debug_map;

    for (const auto& section : pt) {
        for (const auto& entry : section.second) {
            std::string key = entry.first;
            auto value = entry.second.get_value<std::string>();

            if (section.first == "Namespaces") {
                namespace_map[key] = value;
            } else if (section.first == "Cgroup Settings") {
                cgroup_map[key] = value;
            } else if (section.first == "Debug Settings") {
                debug_map[key] = value;
            }
        }
    }

    return std::make_tuple(namespace_map, cgroup_map, debug_map);
}
