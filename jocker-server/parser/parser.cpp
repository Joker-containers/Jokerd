//
// Created by Nazar Demchuk on 28.11.2023.
//

#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "parser.h"

Parser::Parser(const std::string &file_name) : file_name(file_name) {}

container_options Parser::parse_options() {
    // trying to open a file
    std::ifstream file(file_name);

    if (!file) {
        file.close();
        throw std::ios_base::failure("Error opening file");
    }

    // reading contents to a string to use it later
    std::stringstream ss;
    ss << file.rdbuf();

    auto options = parse_json(ss);

    file.close(); // success case
    return options;
}

container_options Parser::parse_json(std::stringstream &ss) {
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);

    // NAMESPACE OPTIONS
    boost::property_tree::ptree ns_options_tree = pt.get_child("namespace_options");

    boost::property_tree::ptree m_ns_collection_tree = ns_options_tree.get_child("m_ns_collection");
    std::vector<std::string> m_ns_collection;
    for (const auto& ns : m_ns_collection_tree) {
        m_ns_collection.push_back(ns.second.get_value<std::string>());
    }

    ns_options namespace_options(m_ns_collection);

    // CGROUP OPTIONS

    boost::property_tree::ptree cgroup_options_tree = pt.get_child("cgroup_options_m");


    // BIN ARGS
    boost::property_tree::ptree bin_arguments_tree = pt.get_child("bin_arguments");
    std::vector<std::string> bin_arguments;
    for (const auto& argument : bin_arguments_tree) {
        bin_arguments.push_back(argument.second.get_value<std::string>());
    }

    // CONTAINER NAME
    auto container_name = pt.get<std::string>("container_name");

    return container_options{namespace_options, {}, bin_arguments, container_name, ""};
}
