#ifndef JOCKER_SERVER_MNT_NS_H
#define JOCKER_SERVER_MNT_NS_H


#include "ns.h"

class mnt_ns: public ns {
public:
    mnt_ns() = delete;

    mnt_ns(std::string name, int fd, pid_t process_pid);

    void setup_ns(const ns_options &opts) override;

    void configure_ns(const ns_options &opts) override;
};


#endif //JOCKER_SERVER_MNT_NS_H
