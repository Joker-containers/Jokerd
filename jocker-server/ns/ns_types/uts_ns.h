#ifndef JOCKER_SERVER_UTS_NS_H
#define JOCKER_SERVER_UTS_NS_H


#include "ns.h"

class uts_ns: public ns {
public:
    uts_ns() = delete;

    uts_ns(std::string name, int fd, pid_t process_pid);

    void setup_ns(const ns_options &opts) override;

    void configure_ns(const ns_options &opts) override;
};


#endif //JOCKER_SERVER_UTS_NS_H
