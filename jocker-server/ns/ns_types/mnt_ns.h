#ifndef JOCKER_SERVER_MNT_NS_H
#define JOCKER_SERVER_MNT_NS_H


#include "ns.h"

class mnt_ns: public ns {
public:
    mnt_ns() = delete;

    mnt_ns(std::string name, int fd, ns_type tp, pid_t process_pid);

    void external_setup_ns(const ns_conf_repository &opts) override;

    void internal_setup_ns(const ns_conf_repository &opts) override;

    void init_internal(const ns_conf_repository &opts) override;

    void configure_ns(const ns_conf_repository &opts) override;
};


#endif //JOCKER_SERVER_MNT_NS_H
