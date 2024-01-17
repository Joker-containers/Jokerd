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

inline constexpr int INVALID_FD = -1;

class binary_exists_error: public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

inline const std::string NAMESPACE_TEMPLATE = "Namespace-template";
inline const std::string NAMESPACE = "Namespace";
inline const std::string CGROUP = "Cgroup";
inline const std::string EMPTY = "";

inline const std::string ID_PROP = "ID";
inline const std::string NAMESPACE_TYPE = "Namespace type";

inline const std::string NAMESPACE_NAME = "Namespace name";
inline const std::string TEMPLATE_ID = "Template-Id";

inline const std::string CONTAINER_NAME_PROP = "Container name";
inline const std::string IPC_NAMESPACE_NAME_PROP = "IPC namespace name";
inline const std::string USER_NAMESPACE_NAME_PROP = "User namespace name";
inline const std::string MNT_NAMESPACE_NAME_PROP = "Mount namespace name";
inline const std::string PID_NAMESPACE_NAME_PROP = "PID namespace name";
inline const std::string NET_NAMESPACE_NAME_PROP = "Network namespace name";
inline const std::string TIME_NAMESPACE_NAME_PROP = "Time namespace name";
inline const std::string UTS_NAMESPACE_NAME_PROP = "UTS namespace name";

inline const std::string CGROUP_NAME = "Cgroup name";
inline const std::string READ_BPS = "Read bps";
inline const std::string WRITE_BPS = "Write bps";
inline const std::string CFS_QUOTA = "Cfs quota";
inline const std::string CFS_PERIOD = "Cfs period";
inline const std::string MEMORY_HIGH = "Memory high";
inline const std::string MEMORY_MAX = "Memory max";
inline const std::string SWAP_MAX = "Swap max";
inline const std::string PIDS_MAX = "Pids max";

struct container_parsed_opts {
    explicit container_parsed_opts(std::ifstream &file);

    container_parsed_opts() = default;

    std::string bin_path = "ip";
    std::vector<std::string> bin_args = {"link", "list"};
    std::string container_name = "FIRST CONTAINER";
    std::string cgroup_name;
    ns_options ns_opt = ns_options();
};

/*
 * Changing the order here requires changes on the client, so be careful
 */
enum request_type{
    RUN_CONTAINER, // 0
    SEND_TRACE,    // 1
    SEND_LOGS,     // 2
    GET_CONFIGS    // 3
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
    uint8_t current_request{};
    uint16_t port;
    int client_socket; // TODO: change to raii fd
    daemon_socket server_socket;
    ns_pool pool;
    ns_conf_repository repo;
    cgroup_manager manager{};

    void get_configs();
};

#endif //JOCKER_SERVER_DAEMON_H
