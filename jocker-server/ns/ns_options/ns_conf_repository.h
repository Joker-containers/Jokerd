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
    ns_conf_repository();

    void add_config(int ns_type, std::shared_ptr<ns_config> &&conf);

    std::shared_ptr<ns_config> find_config(int ns_type, int id);

    std::vector<std::vector<std::shared_ptr<ns_config>>> all_configs;
};


#endif //JOCKER_SERVER_NS_CONF_REPOSITORY_H
