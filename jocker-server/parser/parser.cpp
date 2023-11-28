//
// Created by Nazar Demchuk on 28.11.2023.
//

#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "parser.h"
#include "ns_types/ns_type.h"

Parser::Parser(const std::string &file_name) : file_name(file_name) {}

container_options Parser::parse_options() {
    // trying to open a file
    std::ifstream file(file_name);

    if (!file) {
        file.close();
        throw std::ios_base::failure("Error opening file");
    }

    // reading contents to a string to use it later
    std::stringstream buffer;
    buffer << file.rdbuf();


    container_options options;
    boost::property_tree::ptree pt;

    boost::property_tree::ptree personTree = pt.get_child("person");


    file.close(); // success case
    return {};
}
