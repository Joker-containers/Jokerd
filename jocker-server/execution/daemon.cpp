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
    // Getting binaries and configs
    std::string binary_name, config_filename;
    try{
        std::tie(binary_name, config_filename) = prepare_container_resources();
    }
    catch (binary_exists_error &e){
        return;
    }

    // ======================================================================
    // Parsing config file

    std::ifstream file(config_filename);
    auto opts = parse_container_config(file);

    // ======================================================================
    // Opening logs file

    std::string container_name;
    int logs_fd = syscall_wrapper(open, "open", container_name.c_str(), O_CREAT | O_RDWR); // TODO: move this in container constructor



    // ======================================================================\
    //
    // Creating a container

    /**
     * take shared pointers to needed namespaces from pool
     * form ns group
     * construct container options
     * launch container
    */

    cgroup_manager manager = cgroup_manager();
    auto resources = d_resources(manager);
    auto container_namespaces = pool.get_ns_group(opts.ns_opt);
    container_options opt = container_options(container_namespaces, opts.bin_args, opts.bin_path, container_name, logs_fd);
    container c = container(opt, resources);
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

/**
* Configs parser
 *
 * parse line
 * if line is ns template
 *      add this template to repository
 * else if line is ns
 *      create ns by this template and add to ns pool (after 'return' probably)
 * else if line is cgroup
 *      tbd
 * else
 *      error
*/


void Daemon::parse_ns_template(std::ifstream &file){
    std::string line;
    int ns_t;
    int template_id;
    std::getline(file, line);

    auto [template_id_prop, template_id_str] = parse_variable(line);
    if (template_id_prop != ID_PROP){
        throw std::runtime_error("Bad config!");
    }
    try {
        template_id = stoi(template_id_str);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Bad config!");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Bad config!");
    }

    auto [ns_type_prop, ns_type_string] = parse_variable(line);
    if (ns_type_prop != NAMESPACE_TYPE){
        throw std::runtime_error("Bad config!");
    }
    try {
        ns_t = stoi(ns_type_string);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Bad config!");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Bad config!");
    }

    std::shared_ptr<ns_config> new_ns_conf;
    switch (ns_t){
        case USER:
            new_ns_conf = std::make_shared<user_ns_config>(file, template_id);
            break;
        case MOUNT:
            new_ns_conf = std::make_shared<mnt_ns_config>(file, template_id);
            break;
        case PID:
            new_ns_conf = std::make_shared<pid_ns_config>(file, template_id);
            break;
        case IPC:
            new_ns_conf = std::make_shared<ipc_ns_config>(file, template_id);
            break;
        case NETWORK:
            new_ns_conf = std::make_shared<net_ns_config>(file, template_id);
            break;
        case TIME:
            new_ns_conf = std::make_shared<time_ns_config>(file, template_id);
            break;
        case UTS:
            new_ns_conf = std::make_shared<uts_ns_config>(file, template_id);
            break;
        default:
            break;
    }

    repo.add_config(ns_t, std::move(new_ns_conf));
}

void Daemon::parse_namespace(std::ifstream &file){
    std::string line;

    int ns_t;
    int template_id;

    std::getline(file, line);

    auto [name_prop, name] = parse_variable(line);
    if (name_prop != NAMESPACE_NAME || name.empty()){
        throw std::runtime_error("Bad config!");
    }

    auto [template_id_prop, template_id_str] = parse_variable(line);
    if (template_id_prop != TEMPLATE_ID){
        throw std::runtime_error("Bad config!");
    }
    try {
        template_id = stoi(template_id_str);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Bad config!");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Bad config!");
    }

    auto [ns_type_prop, ns_type_string] = parse_variable(line);
    if (ns_type_prop != NAMESPACE_TYPE){
        throw std::runtime_error("Bad config!");
    }
    try {
        ns_t = stoi(ns_type_string);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Bad config!");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Bad config!");
    }


    pool.declare_ns(static_cast<ns_type>(ns_t), repo.find_config(ns_t, template_id)->create_ns(name));
}

void Daemon::parse_cgroup(std::ifstream &file){
    // TODO
}


void Daemon::parse_config(const std::string &file) {
    std::ifstream input_file(file);
    if (!input_file.is_open()) {
        throw std::runtime_error("Failed to open config file!");
    }

    std::string line;

    while (std::getline(input_file, line)) {
        if (line == NAMESPACE_TEMPLATE){
            parse_ns_template(input_file);
        }
        else if (line == NAMESPACE){
            parse_namespace(input_file);
        }
        else if (line == CGROUP){
            parse_cgroup(input_file);
        }
        else if (line == EMPTY){
            continue;
        }
        else{
            throw std::runtime_error("Bad config!");
        }
    }

    input_file.close();
}

container_parsed_opts Daemon::parse_container_config(std::ifstream &file) {
    std::string line;

    std::string container_name;
    std::string uts_ns_name;
    std::string user_ns_name;
    std::string mnt_ns_name;
    std::string pid_ns_name;
    std::string ipc_ns_name ;
    std::string net_ns_name;
    std::string time_ns_name;
    std::string prop;

    container_parsed_opts opts = container_parsed_opts();

    std::getline(file, line);

    std::tie(prop, container_name) = parse_variable(line);
    if (prop != CONTAINER_NAME_PROP || container_name.empty()){
        throw std::runtime_error("Bad config!");
    }

    std::getline(file, line);
    std::tie(prop, ipc_ns_name) = parse_variable(line);
    if (prop != IPC_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!ipc_ns_name.empty()){
        opts.ns_opt.add_namespace(IPC, ipc_ns_name);
    }

    std::getline(file, line);
    std::tie(prop, user_ns_name) = parse_variable(line);
    if (prop != USER_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!user_ns_name.empty()){
        opts.ns_opt.add_namespace(USER, user_ns_name);
    }

    std::getline(file, line);
    std::tie(prop, mnt_ns_name) = parse_variable(line);
    if (prop != MNT_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!mnt_ns_name.empty()){
        opts.ns_opt.add_namespace(MOUNT, mnt_ns_name);
    }

    std::getline(file, line);
    std::tie(prop, pid_ns_name) = parse_variable(line);
    if (prop != PID_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!pid_ns_name.empty()){
        opts.ns_opt.add_namespace(PID, pid_ns_name);
    }

    std::getline(file, line);
    std::tie(prop, net_ns_name) = parse_variable(line);
    if (prop != NET_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!net_ns_name.empty()){
        opts.ns_opt.add_namespace(NETWORK, net_ns_name);
    }

    std::getline(file, line);
    std::tie(prop, time_ns_name) = parse_variable(line);
    if (prop != TIME_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!time_ns_name.empty()){
        opts.ns_opt.add_namespace(TIME, time_ns_name);
    }

    std::getline(file, line);
    std::tie(prop, uts_ns_name) = parse_variable(line);
    if (prop != UTS_NAMESPACE_NAME_PROP){
        throw std::runtime_error("Bad config!");
    }
    if (!uts_ns_name.empty()){
        opts.ns_opt.add_namespace(UTS, uts_ns_name);
    }

    return opts;
}


/**
* Container options parser
 *
 * parse vars
 * check for unknown
 * check for absent
 * set options
 *
 * return ns options
 *
*/
container_parsed_opts::container_parsed_opts(std::ifstream &file) {
    // Constructor just for a mock object

    std::string uts_ns_name = "uts_test";
    std::string user_ns_name = "user_test";
    std::string mnt_ns_name = "mnt_test";
    std::string pid_ns_name = "pid_test";
    std::string ipc_ns_name = "ipc_test";
    std::string net_ns_name = "net_test";

    ns_opt.add_namespace(UTS, uts_ns_name);
    ns_opt.add_namespace(USER, user_ns_name);
    ns_opt.add_namespace(MOUNT, mnt_ns_name);
    ns_opt.add_namespace(PID, pid_ns_name);
    ns_opt.add_namespace(IPC, ipc_ns_name);
    ns_opt.add_namespace(NETWORK, net_ns_name);
}

