#ifndef JOCKER_SERVER_NS_POOL_H
#define JOCKER_SERVER_NS_POOL_H

#include "ns_types/ipc_ns.h"
#include "ns_types/net_ns.h"
#include "ns_types/mnt_ns.h"
#include "ns_types/pid_ns.h"
#include "ns_types/time_ns.h"
#include "ns_types/user_ns.h"
#include "ns_types/uts_ns.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>

class ns_pool {
public:
    std::shared_ptr<ns> get_ns(ns_type tp, const std::string &ns_name){
        std::shared_ptr<ns> result = nullptr; // In case namespace with such a name is not found, function returns nullptr
        auto it = ns_type_map.find(tp);
        if (it != ns_type_map.end()) {
            auto &tp_ns_collection = it->second;
            for (auto &ns_entry: tp_ns_collection){
                if (ns_entry->get_name() == ns_name){
                    result = ns_entry;
                }
            }
        } else {
            throw std::runtime_error("Unexpected namespace type queried from ns_pool!");
        }
        return result;
    }
    
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
