#ifndef JOCKER_SERVER_PID_NS_CONFIG_H
#define JOCKER_SERVER_PID_NS_CONFIG_H

#include "ns_config.h"
#include <iostream>

class pid_ns_config: public ns_config {
public:
    explicit pid_ns_config(std::ifstream &file, int id);
};


#endif //JOCKER_SERVER_PID_NS_CONFIG_H
