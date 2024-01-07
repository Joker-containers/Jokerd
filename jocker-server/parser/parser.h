//
// Created by Nazar Demchuk on 28.11.2023.
//

#ifndef JOCKER_SERVER_PARSER_H
#define JOCKER_SERVER_PARSER_H

#include <container_options.h>

class Parser {
public:
    explicit Parser(const std::string &file_name);
    container_options parse_options(const std::string& binary_name);
    std::tuple<std::unordered_map<std::string, std::string>,
            std::unordered_map<std::string, std::string>,
            std::unordered_map<std::string, std::string>> parse_ini(const std::ifstream& file);

private:
    const std::string& file_name;
};

#endif //JOCKER_SERVER_PARSER_H
