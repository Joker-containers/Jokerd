//
// Created by Nazar Demchuk on 28.11.2023.
//

#ifndef JOCKER_SERVER_PARSER_H
#define JOCKER_SERVER_PARSER_H

#include <container_options.h>

class Parser {
public:
    explicit Parser(const std::string &file);
    container_options parse_options();
    ns_options parse_ns_options(const std::string& jsonString);

private:
    const std::string& file_name;
};

#endif //JOCKER_SERVER_PARSER_H
