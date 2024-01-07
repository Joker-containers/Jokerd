#ifndef JOCKER_SERVER_USER_NS_H
#define JOCKER_SERVER_USER_NS_H


#include "ns.h"

class user_ns: public ns {
public:
    user_ns() = delete;

    user_ns(std::string name, int fd, ns_type tp, pid_t process_pid);

    void external_setup_ns(const ns_conf_repository &opts) override;

    void internal_setup_ns(const ns_conf_repository &opts) override;

    void init_internal(const ns_conf_repository &opts) override;

    void init_external(const ns_conf_repository &opts) override;

    void configure_ns(const ns_conf_repository &opts) override;
};


#endif //JOCKER_SERVER_USER_NS_H
