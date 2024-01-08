//
// Created by Nazar Demchuk on 06.01.2024.
//

#ifndef JOCKER_SERVER_DAEMON_H
#define JOCKER_SERVER_DAEMON_H

#include <netinet/in.h>
#include <cstdint>
#include <vector>
#include <fstream>
#include "ns_pool.h"
#include "ns_options/ns_conf_repository.h"

constexpr int INVALID_FD = -1;

class Daemon {
public:
    Daemon(uint16_t port, const std::string &log_file_path);

    virtual ~Daemon();

    void check_if_logs_opened();

    void setup_sockets();

    void get_request_type();

    void execute_request();

    void send_logs();

    void send_trace();

    void run_container();

    void log_message(const std::string& message, bool to_cerr = false);

    std::tuple<std::vector<char>, std::vector<char>, std::vector<char>> get_run_data();

private:
    uint16_t port;
    std::ofstream log_file_writer;
    std::ifstream log_file_reader;
    uint8_t current_request;
    int client_socket;
    int server_socket;
    sockaddr_in server_addr;
    sockaddr_in client_addr;
    ns_pool pool;
    ns_conf_repository repo;
};
#endif //JOCKER_SERVER_DAEMON_H
