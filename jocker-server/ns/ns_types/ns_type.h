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

const std::map<std::string, int> ns_type_strings {
        {"IPC", IPC},
        {"NETWORK", NETWORK},
        {"MOUNT", MOUNT},
        {"PID", PID},
        {"TIME", TIME},
        {"USER", USER},
        {"UTS", UTS},
};

#endif //JOCKER_SERVER_NS_TYPE_H
