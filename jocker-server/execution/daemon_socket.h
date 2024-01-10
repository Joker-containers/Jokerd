#ifndef JOCKER_SERVER_DAEMON_SOCKET_H
#define JOCKER_SERVER_DAEMON_SOCKET_H

#include "syscall_wrapper.h"
#include <string>
#include <stdexcept>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>
#include <netinet/tcp.h>

constexpr int ERROR_CODE = -1;

class daemon_socket {
public:
    explicit daemon_socket(ssize_t port);

    void server_listen();

    [[nodiscard]] int server_accept() const;

    ~daemon_socket();

private:
    bool valid_socket = false;
    int binded_socket = -1;
};


#endif //JOCKER_SERVER_DAEMON_SOCKET_H
