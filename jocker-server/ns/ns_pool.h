#ifndef JOCKER_SERVER_NS_POOL_H
#define JOCKER_SERVER_NS_POOL_H

#include "ns_types/ipc_ns.h"
#include "ns_types/net_ns.h"
#include "ns_types/mnt_ns.h"
#include "ns_types/pid_ns.h"
#include "ns_types/time_ns.h"
#include "ns_types/user_ns.h"
#include "ns_types/uts_ns.h"
#include "ns_types/ns_type.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

class ns_pool {
public:
    bool exists_ns(ns_type tp, const std::string &ns_name);

    void register_ns(ns_type tp, const std::shared_ptr<ns>& to_register);

    std::shared_ptr<ns> get_ns(ns_type tp, const std::string &ns_name);

    bool delete_ns(ns_type tp, const std::string &ns_name);

private:
    std::unordered_map<ns_type, std::vector<std::shared_ptr<ns>> &> ns_type_map{
            {IPC, all_ipc_ns},
            {NETWORK, all_net_ns},
            {MOUNT, all_mnt_ns},
            {PID, all_pid_ns},
            {TIME, all_time_ns},
            {USER, all_user_ns},
            {UTS, all_uts_ns}
    };

    // Making this members of type std::vector<std::shared_ptr<ns_type>> would be better,
    // but that brings many problems
    std::vector<std::shared_ptr<ns>> all_ipc_ns;
    std::vector<std::shared_ptr<ns>> all_net_ns;
    std::vector<std::shared_ptr<ns>> all_mnt_ns;
    std::vector<std::shared_ptr<ns>> all_pid_ns;
    std::vector<std::shared_ptr<ns>> all_time_ns;
    std::vector<std::shared_ptr<ns>> all_user_ns;
    std::vector<std::shared_ptr<ns>> all_uts_ns;
};


#endif //JOCKER_SERVER_NS_POOL_H
