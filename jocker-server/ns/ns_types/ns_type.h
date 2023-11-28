#ifndef JOCKER_SERVER_NS_TYPE_H
#define JOCKER_SERVER_NS_TYPE_H

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

#endif //JOCKER_SERVER_NS_TYPE_H
