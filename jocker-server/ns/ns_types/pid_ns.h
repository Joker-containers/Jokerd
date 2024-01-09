#ifndef JOCKER_SERVER_PID_NS_H
#define JOCKER_SERVER_PID_NS_H


#include "ns.h"

class pid_ns: public ns {
public:
    pid_ns() = delete;

    explicit pid_ns(std::string name);

    pid_ns(std::string name, int fd, pid_t process_pid);

    void configure_ns() override;

    void internal_setup_ns() override;

    void external_setup_ns() override;

    void init_internal() override;

    void init_external() override;

    [[nodiscard]] int get_type() const override{
        return PID;
    }
};


#endif //JOCKER_SERVER_PID_NS_H
