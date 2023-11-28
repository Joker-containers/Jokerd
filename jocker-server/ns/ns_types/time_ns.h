#ifndef JOCKER_SERVER_TIME_NS_H
#define JOCKER_SERVER_TIME_NS_H


#include "ns.h"

class time_ns: public ns {
public:
    time_ns() = delete;

    time_ns(std::string name, int fd, pid_t process_pid);

    void setup_ns(const ns_options &opts) override;

    void configure_ns(const ns_options &opts) override;
};


#endif //JOCKER_SERVER_TIME_NS_H
