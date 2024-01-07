#ifndef JOCKER_SERVER_PID_NS_H
#define JOCKER_SERVER_PID_NS_H


#include "ns.h"

class pid_ns: public ns {
public:
    pid_ns() = delete;

    pid_ns(std::string name, int fd, ns_type tp, pid_t process_pid);

    void configure_ns(const ns_conf_repository &opts) override;

    void internal_setup_ns(const ns_conf_repository &opts) override;

    void external_setup_ns(const ns_conf_repository &opts) override;

    void init_internal(const ns_conf_repository &opts) override;

    void init_external(const ns_conf_repository &opts) override;
};


#endif //JOCKER_SERVER_PID_NS_H
