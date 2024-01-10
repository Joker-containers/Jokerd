#include "daemon_socket.h"

daemon_socket::daemon_socket(ssize_t port){
    struct sockaddr_in server{};
    binded_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (binded_socket == ERROR_CODE) {
        throw std::runtime_error("Error: Unable to create socket.");
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(binded_socket, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) == ERROR_CODE) {
        close(binded_socket);
        throw std::runtime_error("Error: Unable to bind socket.");
    }
    valid_socket = true;
}

void daemon_socket::server_listen() {
    if (!valid_socket){
        throw std::runtime_error("Trying to listen to connections using invalid socket!");
    }
    if (listen(binded_socket, 1) == ERROR_CODE){
        close(binded_socket);
        valid_socket = false;
        throw std::runtime_error("Error: Unable to listen on socket.");
    }
}

int daemon_socket::server_accept() const{
    if (!valid_socket){
        throw std::runtime_error("Trying to accept connection using invalid socket!");
    }
    int client_socket = accept(binded_socket, nullptr, nullptr);
    if (client_socket == ERROR_CODE){
        throw std::runtime_error("Failed to accept connection!");
    }


    int flag = 1;
    // Disable Nagle algorithm
    syscall_wrapper(setsockopt, "setsockopt", client_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

    return client_socket;
}

daemon_socket::~daemon_socket(){
    if (valid_socket){
        close(binded_socket);
    }
}