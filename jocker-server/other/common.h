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


#endif //JOCKER_SERVER_COMMON_H
