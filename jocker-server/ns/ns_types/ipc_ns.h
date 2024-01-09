#ifndef JOCKER_SERVER_IPC_NS_H
#define JOCKER_SERVER_IPC_NS_H


#include "ns.h"

class ipc_ns: public ns {
public:
    ipc_ns() = delete;

    explicit ipc_ns(std::string name);

    ipc_ns(std::string name, int fd, pid_t process_pid);

    void external_setup_ns() override;

    void configure_ns() override;

    [[nodiscard]] int get_type() const override{
        return IPC;
    }
};


#endif //JOCKER_SERVER_IPC_NS_H
