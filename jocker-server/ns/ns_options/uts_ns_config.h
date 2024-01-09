#ifndef JOCKER_SERVER_UTS_NS_CONFIG_H
#define JOCKER_SERVER_UTS_NS_CONFIG_H

#include "ns_config.h"
#include <string>
#include <iostream>

class uts_ns_config: public ns_config {
public:
    explicit uts_ns_config(std::ifstream &file, int id);

    std::shared_ptr<ns> create_ns(const std::string &name) override;

    std::string hostname;
};


#endif //JOCKER_SERVER_UTS_NS_CONFIG_H
