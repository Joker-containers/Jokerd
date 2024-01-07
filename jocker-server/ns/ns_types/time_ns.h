#ifndef JOCKER_SERVER_TIME_NS_H
#define JOCKER_SERVER_TIME_NS_H


#include "ns.h"

class time_ns: public ns {
public:
    time_ns() = delete;

    time_ns(std::string name, int fd, ns_type tp, pid_t process_pid);

    void external_setup_ns(const ns_conf_repository &opts) override;

    void configure_ns(const ns_conf_repository &opts) override;
};


#endif //JOCKER_SERVER_TIME_NS_H
