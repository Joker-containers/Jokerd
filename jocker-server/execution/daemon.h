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
#include "daemon_socket.h"
#include "cgroup_manager.h"

constexpr int INVALID_FD = -1;

class binary_exists_error: public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

const std::string NAMESPACE_TEMPLATE = "Namespace-template";
const std::string NAMESPACE = "Namespace";
const std::string CGROUP = "Cgroup";
const std::string EMPTY = "";

const std::string ID_PROP = "ID";
const std::string NAMESPACE_TYPE = "Namespace type";

const std::string NAMESPACE_NAME = "Namespace name";
const std::string TEMPLATE_ID = "Template-Id";


const std::string CONTAINER_NAME_PROP = "Container name";
const std::string IPC_NAMESPACE_NAME_PROP = "IPC namespace name";
const std::string USER_NAMESPACE_NAME_PROP = "User namespace name";
const std::string MNT_NAMESPACE_NAME_PROP = "Mount namespace name";
const std::string PID_NAMESPACE_NAME_PROP = "PID namespace name";
const std::string NET_NAMESPACE_NAME_PROP = "Network namespace name";
const std::string TIME_NAMESPACE_NAME_PROP = "Time namespace name";
const std::string UTS_NAMESPACE_NAME_PROP = "UTS namespace name";

const std::string CGROUP_NAME = "Cgroup name";
const std::string READ_BPS = "Read bps";
const std::string WRITE_BPS = "Write bps";
const std::string CFS_QUOTA = "Cfs quota";
const std::string CFS_PERIOD = "Cfs period";
const std::string MEMORY_HIGH = "Memory high";
const std::string MEMORY_MAX = "Memory max";
const std::string SWAP_MAX = "Swap max";
const std::string PIDS_MAX = "Pids max";

struct container_parsed_opts {
    explicit container_parsed_opts(std::ifstream &file);

    container_parsed_opts() = default;

    std::string bin_path = "ip";
    std::vector<std::string> bin_args = {"link", "list"};
    std::string container_name = "FIRST CONTAINER";
    std::string cgroup_name;
    ns_options ns_opt = ns_options();
};

class Daemon {
public:
    Daemon(uint16_t port, const std::string &log_file_path);

    virtual ~Daemon();

    void check_if_logs_opened();

    bool get_request_type();

    void execute_request();

    void send_logs();

    void send_trace();

    void run_container();

    void parse_ns_template(std::ifstream &file);

    void parse_config(const std::string &file);

    void parse_namespace(std::ifstream &file);

    void parse_cgroup(std::ifstream &file);

    static container_parsed_opts parse_container_config(std::ifstream &file);

    std::pair<std::string, std::string> prepare_container_resources();

    void log_message(const std::string& message, bool to_cerr = false);

    std::tuple<std::vector<char>, std::vector<char>, std::vector<char>> get_run_data();

    void reconnect();

private:
    std::ofstream log_file_writer;
    std::ifstream log_file_reader;
    uint8_t current_request;
    uint16_t port;
    int client_socket; // TODO: change to raii fd
    daemon_socket server_socket;
    ns_pool pool;
    ns_conf_repository repo;
    cgroup_manager manager{};

    void get_configs();
};

#endif //JOCKER_SERVER_DAEMON_H
