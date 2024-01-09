#ifndef JOCKER_SERVER_TIME_NS_H
#define JOCKER_SERVER_TIME_NS_H


#include "ns.h"

class time_ns: public ns {
public:
    time_ns() = delete;

    explicit time_ns(std::string name);

    time_ns(std::string name, int fd, pid_t process_pid);

    void external_setup_ns() override;

    void configure_ns() override;

    [[nodiscard]] int get_type() const override{
        return TIME;
    }
};


#endif //JOCKER_SERVER_TIME_NS_H
