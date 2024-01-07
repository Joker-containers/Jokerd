#ifndef JOCKER_SERVER_NS_TYPE_H
#define JOCKER_SERVER_NS_TYPE_H

#include <map>
#include <string>
#include <unordered_map>

/* Enumeration of all namespaces. IMPORTANT: NS_TYPES_NUM should always be the last type,
 * as some functions rely on that property. Violating this rule may lead to dire things to happen. */
enum ns_type{
    USER,
    MOUNT,
    PID,
    IPC,
    NETWORK,
    TIME,
    UTS,
    NS_TYPES_NUM
};

const std::map<std::string, int> ns_type_strings {
        {"USER", USER},
        {"MOUNT", MOUNT},
        {"PID", PID},
        {"IPC", IPC},
        {"NETWORK", NETWORK},
        {"TIME", TIME},
        {"UTS", UTS},
};

const std::unordered_map<ns_type, int> NS_CLONE_FLAG = {
        {IPC,     CLONE_NEWIPC},
        {NETWORK, CLONE_NEWNET},
        {MOUNT,   CLONE_NEWNS},
        {PID,     CLONE_NEWPID},
        // {TIME, CLONE_NEWTIME}, // It seems that it is not supported on my Linux version
        {USER, CLONE_NEWUSER},
        {UTS, CLONE_NEWUTS},
};

const std::unordered_map<ns_type, std::string> NS_SHORTCUT = {
        {USER, "user"},
        {MOUNT, "mnt"},
        {PID, "pid"},
        {IPC, "ipc"},
        {NETWORK, "net"},
        {TIME, "time"},
        {UTS, "uts"}
};

#endif //JOCKER_SERVER_NS_TYPE_H
