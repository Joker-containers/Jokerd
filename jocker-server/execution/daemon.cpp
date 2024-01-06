//
// Created by Nazar Demchuk on 06.01.2024.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <mutex>
#include <cstdint>
#include <tuple>
#include "daemon.h"


//void get_file_and_config(int client_socket) {
//    std::ofstream logsFile("containers.log", std::ios::app);
//    if (!logsFile.is_open()) {
//        std::cerr << "Error: Unable to open logs file." << std::endl;
//        return;
//    }
//
//    log_message("Starting to receive container files...");
//
//    log_message("\nReceiving binary filename size...");
//    uint64_t binary_name_size;
//    recv(client_socket, &binary_name_size, sizeof(binary_name_size), 0);
//    log_message("Received binary filename length: " + std::to_string(binary_name_size));
//
//    log_message("\nReceiving binary filename...");
//    std::vector<char> binary_name(binary_name_size);
//    recv(client_socket, binary_name.data(), binary_name_size, 0);
//    log_message("Received binary filename: " + std::string(binary_name.data()));
//
//    log_message("\nReceiving binary size...");
//    uint64_t binary_size;
//    recv(client_socket, &binary_size, sizeof(binary_size), 0);
//    log_message("Received binary size: " + std::to_string(binary_size));
//
//    log_message("\nReceiving binary...");
//    std::vector<char> binary(binary_size);
//    recv(client_socket, binary.data(), binary_size, 0);
//    log_message("Received binary.");
//
//    std::string config_filename = std::string(binary_name.data()) + ".joker";
//    log_message("\nConfig filename is: " + config_filename);
//    log_message("Receiving config size...");
//    uint64_t binary_config_size;
//    recv(client_socket, &binary_config_size, sizeof(binary_config_size), 0);
//    log_message("Received config size: " + std::to_string(binary_config_size));
//
//    log_message("\nReceiving config...");
//    std::vector<char> binary_config(binary_config_size);
//    recv(client_socket, binary_config.data(), binary_config_size, 0);
//    log_message("Received config.");
//
//    std::ifstream check_file(binary_name.data());
//    if (check_file.good()) {
//        log_message("\nError: File with the name " + std::string(binary_name.data()) + " already exists.", true);
//        return;
//    }
//    check_file.close();
//
//    log_message("\nSaving binary file...");
//    std::ofstream binary_file(binary_name.data(), std::ios::binary);
//    binary_file.write(binary.data(), binary_size);
//    binary_file.close();
//    log_message("Saved binary file to " + std::string(binary_name.data()) + ".\n");
//
//    log_message("\nSaving config file...");
//    std::ofstream binary_config_file(config_filename, std::ios::binary);
//    binary_config_file.write(binary_config.data(), binary_config_size);
//    binary_config_file.close();
//    log_message("Saved config file to " + config_filename + ".\n");
//}

Daemon::Daemon(uint16_t port, std::string  log_file_path): log_file_path(std::move(log_file_path)) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        log_message("Error: Unable to create socket.", true);
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) == -1) {
        perror("bind");
        log_message("Error: Unable to bind socket.", true);
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        log_message("Error: Unable to listen on socket.", true);
        close(server_socket);
        exit(1);
    }

    log_message("Daemon listening on port " + std::to_string(port));

    socklen_t clientAddrLen = sizeof(client_addr);

    client_socket = accept(server_socket, reinterpret_cast<struct sockaddr *>(&client_addr), &clientAddrLen);
    if (client_socket == -1) {
        log_message("Error: Unable to accept client connection.", true);
        exit(-1);
    }
}

void Daemon::process_request() {
    recv(client_socket, &current_request, sizeof(current_request), 0);
}

void Daemon::execute_request() {
    if (current_request == 0) {
        this->run_container();
    } else if (current_request == 1) {
        this->send_trace();
    } else if (current_request == 2) {
        this->send_logs();
    } else {
        log_message(std::string("Unrecognized request type: ") + std::to_string(current_request), true);
    }
}

void Daemon::send_logs() {

}

void Daemon::send_trace() {
    std::ifstream logsFile(log_file_path, std::ios::binary);
    if (!logsFile.is_open()) {
        std::cerr << "Error: Unable to open logs file." << std::endl;
        return;
    }

    std::vector<char> logsContent((std::istreambuf_iterator<char>(logsFile)),
                                  std::istreambuf_iterator<char>());

    uint64_t logsSize = logsContent.size();
    send(client_socket, logsContent.data(), logsSize, 0);
    shutdown(client_socket, SHUT_WR);

    logsFile.close();
}

void Daemon::run_container() {

}

void Daemon::log_message(const std::string& message, bool to_cerr) {

    if (to_cerr) {
        std::cerr << message << std::endl;
    } else {
        std::cout << message << std::endl;
    }

    std::ofstream logsFile(log_file_path, std::ios::app);
    if (!logsFile.is_open()) {
        std::cerr << "Error: Unable to open logs file." << std::endl;
        return;
    }

    logsFile << message << std::endl;
    logsFile.close();
}

