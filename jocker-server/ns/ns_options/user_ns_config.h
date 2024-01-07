#ifndef JOCKER_SERVER_USER_NS_CONFIG_H
#define JOCKER_SERVER_USER_NS_CONFIG_H


#include <string>
#include <vector>

class user_ns_config {
public:
    std::vector<std::string> uid_mappings;
    std::vector<std::string> gid_mappings;
};


#endif //JOCKER_SERVER_USER_NS_CONFIG_H
