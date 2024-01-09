//
// Created by oleksandr on 09.01.24.
//

#ifndef JOCKER_SERVER_NS_CONFIG_H
#define JOCKER_SERVER_NS_CONFIG_H

#include "ns_types/ns.h"

const std::string PARSE_ERROR_MSG = "Bad config!";

class ns;

class ns_config {
public:
    virtual std::shared_ptr<ns> create_ns() = 0;

    [[nodiscard]] bool compare_id(int id) const{
        if (id == config_id){
            return true;
        }
    }

protected:
    int config_id{};
    void set_id(int id){
        config_id = id;
    }
};


#endif //JOCKER_SERVER_NS_CONFIG_H
