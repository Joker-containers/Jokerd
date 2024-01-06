#ifndef JOCKER_SERVER_UTS_NS_H
#define JOCKER_SERVER_UTS_NS_H


#include "ns.h"
#include <sys/utsname.h>
#include <cstring>
#include <err.h>

class uts_ns: public ns {
public:
    uts_ns() = delete;

    uts_ns(std::string name, int fd, ns_type tp, pid_t process_pid);

    void setup_ns(const ns_conf_repository &opts) override;

    void configure_ns(const ns_conf_repository &opts) override;

    void init_internal(const ns_conf_repository &opts) override;
};


#endif //JOCKER_SERVER_UTS_NS_H
