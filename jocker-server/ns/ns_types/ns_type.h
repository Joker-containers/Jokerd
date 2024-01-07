#ifndef JOCKER_SERVER_NS_TYPE_H
#define JOCKER_SERVER_NS_TYPE_H

#include <map>
#include <string>

/* Enumeration of all namespaces. IMPORTANT: NS_TYPES_NUM should always be the last type,
 * as some functions rely on that property. Violating this rule may lead to dire things to happen. */
enum ns_type{
    IPC,
    NETWORK,
    MOUNT,
    PID,
    TIME,
    USER,
    UTS,
    NS_TYPES_NUM
};

std::map<int, std::string> ns_type_strings {
        {IPC, "ipc_ns_name"},
        {NETWORK, "network_ns_name"},
        {MOUNT, "mount_ns_name"},
        {PID, "pid_ns_name"},
        {TIME, "time_ns_name"},
        {USER, "user_ns_name"},
        {UTS, "uts_ns_name"},
};

#endif //JOCKER_SERVER_NS_TYPE_H
