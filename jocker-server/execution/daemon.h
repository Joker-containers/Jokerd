//
// Created by Nazar Demchuk on 06.01.2024.
//

#ifndef JOCKER_SERVER_DAEMON_H
#define JOCKER_SERVER_DAEMON_H

#include <cstdint>
#include "ns_pool.h"
#include "ns_options/ns_conf_repository.h"

class Daemon {
public:
    explicit Daemon(uint16_t port, const std::string& log_file_path);

    void process_request();

    void execute_request();

    void send_logs();

    void send_trace();

    void run_container();

    static void log_message(const std::string& message, bool to_cerr = false);

private:
    uint16_t port;
    std::string log_file_path;
    uint8_t current_request;
    int client_socket;
    int server_socket;
    sockaddr_in server_addr;
    sockaddr_in client_addr;
    ns_pool pool;
    ns_conf_repository repo;
};

void log_message(const std::string& message, bool to_cerr);

void get_file_and_config(int client_socket);

void send_logs(int client_socket);

uint8_t process_request(int client_socket);

void setup_server();

#endif //JOCKER_SERVER_DAEMON_H
