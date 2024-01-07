#ifndef JOCKER_SERVER_COMMON_H
#define JOCKER_SERVER_COMMON_H

#include <memory>
#include <cstring>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>

struct raii_fd;

void deleteCharPtrArray(char **av);

std::unique_ptr<char *[], void (*)(char **)> createCharPtrArray(const std::vector<std::string> &tokens);

int write_to_file(const std::string &path, const std::string &content);

void recv_all(int socket, char *buffer, size_t size);


#endif //JOCKER_SERVER_COMMON_H
