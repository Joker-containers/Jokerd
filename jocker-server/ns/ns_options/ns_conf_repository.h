#ifndef JOCKER_SERVER_NS_CONF_REPOSITORY_H
#define JOCKER_SERVER_NS_CONF_REPOSITORY_H


#include "ns_types/ns_type.h"
#include "ipc_ns_config.h"
#include "mnt_ns_config.h"
#include "net_ns_config.h"
#include "pid_ns_config.h"
#include "time_ns_config.h"
#include "user_ns_config.h"
#include "uts_ns_config.h"
#include <unordered_map>
#include <string>

class ns_conf_repository {
public:
    std::unordered_map<std::string, ipc_ns_config> ipc_ns_configs;
    std::unordered_map<std::string, mnt_ns_config> mnt_ns_configs;
    std::unordered_map<std::string, net_ns_config> net_ns_configs;
    std::unordered_map<std::string, pid_ns_config> pid_ns_configs;
    std::unordered_map<std::string, time_ns_config> time_ns_configs;
    std::unordered_map<std::string, user_ns_config> user_ns_configs;
    std::unordered_map<std::string, uts_ns_config> uts_ns_configs;
};


#endif //JOCKER_SERVER_NS_CONF_REPOSITORY_H
