#ifndef JOCKER_SERVER_NET_NS_H
#define JOCKER_SERVER_NET_NS_H


#include "ns.h"

class net_ns: public ns {
public:
    net_ns() = delete;

    explicit net_ns(std::string name);

    net_ns(std::string name, int fd, pid_t process_pid);

    void external_setup_ns() override;

    void configure_ns() override;

    [[nodiscard]] int get_type() const override{
        return NETWORK;
    }
};


#endif //JOCKER_SERVER_NET_NS_H
