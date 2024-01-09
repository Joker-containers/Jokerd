#ifndef JOCKER_SERVER_UTS_NS_H
#define JOCKER_SERVER_UTS_NS_H


#include "ns.h"
#include <sys/utsname.h>
#include <cstring>
#include <err.h>

class uts_ns: public ns {
public:
    uts_ns() = delete;

    uts_ns(std::string name, std::string hostname);

    uts_ns(std::string name, int fd, pid_t process_pid);

    void internal_setup_ns() override;

    void configure_ns() override;

    void init_internal() override;

    [[nodiscard]] int get_type() const override{
        return UTS;
    }
private:
    std::string hostname;
};


#endif //JOCKER_SERVER_UTS_NS_H
