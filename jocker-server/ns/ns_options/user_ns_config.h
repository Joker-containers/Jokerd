#ifndef JOCKER_SERVER_USER_NS_CONFIG_H
#define JOCKER_SERVER_USER_NS_CONFIG_H

#include "ns_config.h"
#include <string>
#include <vector>

class user_ns_config: public ns_config {
public:
    explicit user_ns_config(std::ifstream &file, int id);

    std::shared_ptr<ns> create_ns(const std::string &name) override;

    std::vector<std::string> uid_mappings;
    std::vector<std::string> gid_mappings;
};


#endif //JOCKER_SERVER_USER_NS_CONFIG_H
