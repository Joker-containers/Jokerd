#ifndef JOCKER_SERVER_COMMON_H
#define JOCKER_SERVER_COMMON_H


#include "syscall_wrapper.h"
#include <memory>
#include <cstring>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <fstream>

struct raii_fd {
    int fd;

    raii_fd(){
        fd = -1;
    }

    explicit raii_fd(int fd) : fd(fd) {};

    ~raii_fd() {
        syscall_wrapper(close, "close", fd);
    }
};

void deleteCharPtrArray(char **av);

std::unique_ptr<char *[], void (*)(char **)> createCharPtrArray(const std::vector<std::string> &tokens);

int write_to_file(const std::string &path, const std::string &content);

void recv_all(int socket, void *buffer, size_t size, int flags = 0);

void send_all(int socket, void *buffer, size_t size, int flags = 0);

std::pair<std::string, std::string> parse_variable(const std::string &line);

template<typename T>
void check_and_assign_variable(const std::string &expected_variable_name, T& field, std::ifstream &file, const std::string &err_msg){
    std::string line;
    std::getline(file, line);

    auto res = parse_variable(line);
    if (res.first != expected_variable_name){
        throw std::runtime_error(err_msg);
    }
    field = res.second;
}

#endif //JOCKER_SERVER_COMMON_H
