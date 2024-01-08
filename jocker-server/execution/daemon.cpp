//
// Created by Nazar Demchuk on 06.01.2024.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <tuple>
#include <netinet/tcp.h>
#include "daemon.h"
#include "parser.h"
#include "common.h"
#include "container.h"

void Daemon::check_if_logs_opened() {
    if (!log_file_writer.is_open()) {
        std::stringstream err_message;
        err_message << "Error: Unable to open trace file for writing." << std::endl;
        throw std::runtime_error(err_message.str());
    }
    if (!log_file_reader.is_open()) {
        std::stringstream err_message;
        err_message << "Error: Unable to open trace file for reading." << std::endl;
        throw std::runtime_error(err_message.str());
    }
}

void Daemon::setup_sockets() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_FD) {
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

    int flag = 1;

    // Disable Nagle algorithm
    syscall_wrapper(setsockopt, "setsockopt", client_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
}

Daemon::Daemon(uint16_t port, const std::string& log_file_path):
log_file_writer(log_file_path), log_file_reader(log_file_path), port(port)
{
    check_if_logs_opened();
    setup_sockets();
}

void Daemon::get_request_type() {
    recv_all(client_socket, &current_request, sizeof(current_request));
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
    log_message("Starting to receive container to get logs of...");

    log_message("\nReceiving container name size...");
    uint64_t container_name_size;
    recv_all(client_socket, &container_name_size, sizeof(container_name_size));
    log_message("Received container name size: " + std::to_string(container_name_size));

    log_message("\nReceiving container name...");
    std::vector<char> container_name_v(container_name_size);
    recv_all(client_socket, container_name_v.data(), container_name_size);
    log_message("Received container name: " + std::string(container_name_v.data()));

    std::string container_name = container_name_v.data();
    container_name += ".log";
    std::ifstream container(container_name);

    if (!container.is_open()) {
        log_message(std::string("Error: Unable to open logs file for container ") + container_name, true);
        return;
    }

    std::vector<char> logsContent((std::istreambuf_iterator<char>(container)),
                                  std::istreambuf_iterator<char>());

    uint64_t logsSize = logsContent.size();
    send_all(client_socket, &logsSize, sizeof(logsSize));
    send_all(client_socket, logsContent.data(), logsSize);
}

void Daemon::send_trace() {

    std::vector<char> logsContent((std::istreambuf_iterator<char>(log_file_reader)),
                                  std::istreambuf_iterator<char>());
    uint64_t logsSize = logsContent.size();
    send_all(client_socket, &logsSize, sizeof(logsSize));
    send_all(client_socket, logsContent.data(), logsSize);
}

std::tuple<std::vector<char>, std::vector<char>, std::vector<char>> Daemon::get_run_data() {
    log_message("Starting to receive container files...");

    log_message("\nReceiving binary filename size...");
    uint64_t binary_name_size;
    recv_all(client_socket, &binary_name_size, sizeof(binary_name_size));
    log_message("Received binary filename length: " + std::to_string(binary_name_size));

    log_message("\nReceiving binary filename...");
    std::vector<char> binary_name(binary_name_size + 1);
    binary_name[binary_name_size] = '\0';
    recv_all(client_socket, binary_name.data(), binary_name_size);
    log_message("Received binary filename: " + std::string(binary_name.data()));

    log_message("\nReceiving binary size...");
    uint64_t binary_size;
    recv_all(client_socket, &binary_size, sizeof(binary_size));
    log_message("Received binary size: " + std::to_string(binary_size));

    log_message("\nReceiving binary...");
    std::vector<char> binary(binary_size);
    recv_all(client_socket, binary.data(), binary_size);
    log_message("Received binary.");

    std::string config_filename = std::string(binary_name.data()) + ".joker";
    log_message("\nConfig filename is: " + config_filename);

    log_message("Receiving config size...");
    uint64_t binary_config_size;
    recv_all(client_socket, &binary_config_size, sizeof(binary_config_size));
    log_message("Received config size: " + std::to_string(binary_config_size));

    log_message("\nReceiving config...");
    std::vector<char> binary_config(binary_config_size);
    recv_all(client_socket, binary_config.data(), binary_config_size);
    log_message("Received config.");

    return std::make_tuple(binary_name, binary, binary_config);
}

std::pair<std::string, std::string> Daemon::prepare_container_resources() {
    auto [binary_name_v, binary, binary_config] = get_run_data();
    std::string binary_name = binary_name_v.data();
    auto config_filename = binary_name + ".joker";

    std::ifstream check_file(binary_name.data());
    if (check_file.good()) {
        std::string error_message = "\nError: File with the name " + std::string(binary_name) + " already exists.";
        log_message(error_message, true);
        throw binary_exists_error(error_message);
    }
    check_file.close();

    log_message("\nSaving binary file...");
    std::ofstream binary_file(binary_name, std::ios::binary);
    binary_file.write(binary.data(), (std::streamsize)binary.size());
    binary_file.close();
    log_message("Saved binary file to " + std::string(binary_name) + ".\n");

    log_message("\nSaving config file...");
    std::ofstream config_file(config_filename, std::ios::binary);
    config_file.write(binary_config.data(), (std::streamsize)binary_config.size());
    config_file.close();
    log_message("Saved config file to " + config_filename + ".\n");

    return {binary_name, config_filename};
}


void Daemon::run_container() {
    // ======================================================================
    // Creating binaries
    std::string binary_name, config_filename;
    try{
        std::tie(binary_name, config_filename) = prepare_container_resources();
    }
    catch (binary_exists_error &e){
        return;
    }

    // ======================================================================
    // Parsing config file

    std::string uts_ns_name = "uts_test";
    std::string user_ns_name = "user_test";
    std::string mnt_ns_name = "mnt_test";
    std::string pid_ns_name = "pid_test";
    std::string ipc_ns_name = "ipc_test";
    std::string net_ns_name = "net_test";

    std::string uts_hostname = "TESTMACHINE";
    uts_ns_config uts_conf = uts_ns_config(uts_hostname);
    user_ns_config user_conf = {};
    mnt_ns_config mnt_conf = {.new_rootfs_path = "./data/alpine_rootfs"};
    pid_ns_config pid_conf;
    ipc_ns_config ipc_conf;
    net_ns_config net_conf;

    repo.uts_ns_configs[uts_ns_name] = uts_conf;
    repo.user_ns_configs[user_ns_name] = user_conf;
    repo.mnt_ns_configs[mnt_ns_name] = mnt_conf;
    repo.pid_ns_configs[pid_ns_name] = pid_conf;
    repo.ipc_ns_configs[ipc_ns_name] = ipc_conf;
    repo.net_ns_configs[net_ns_name] = net_conf;

    ns_options ns_opt = ns_options();

    ns_opt.add_namespace(UTS, uts_ns_name);
    ns_opt.add_namespace(USER, user_ns_name);
    ns_opt.add_namespace(MOUNT, mnt_ns_name);
    ns_opt.add_namespace(PID, pid_ns_name);
    ns_opt.add_namespace(IPC, ipc_ns_name);
    ns_opt.add_namespace(NETWORK, net_ns_name);

    std::string bin_path = "ip";

    std::vector<std::string> bin_args = {"link", "list"};

    std::string container_name = "FIRST CONTAINER";

    int logs_fd = syscall_wrapper(open, "open", container_name.c_str(), O_CREAT | O_RDWR); // TODO: move this in container constructor

    container_options opt = container_options(ns_opt, bin_args, bin_path, container_name, logs_fd);
    
//    Parser parser(config_filename);
//    auto options = parser.parse_options(binary_name);

    // ======================================================================\
    // TODO: ADD FUCKING CONTAINER CREATION. THE FUCK YOU'RE LOOKING AT???
    // Creating a container
    // ...

    auto res = d_resources(pool, repo);
    container c = container(opt, res);
    sleep(5);
}

void Daemon::log_message(const std::string& message, bool to_cerr) {

    if (to_cerr) {
        std::cerr << message << std::endl;
    } else {
        std::cout << message << std::endl;
    }

    log_file_writer << message << std::endl;
}

Daemon::~Daemon() {
    if (client_socket != INVALID_FD) {
        close(client_socket);
        // log_message("Closed client socket.");
    }

    if (server_socket != INVALID_FD) {
        close(server_socket);
        // log_message("Closed server socket.");
    }
    log_file_reader.close();
    log_file_writer.close();
}
