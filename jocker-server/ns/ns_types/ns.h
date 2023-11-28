#ifndef JOCKER_SERVER_NS_H
#define JOCKER_SERVER_NS_H

#include "ns_options/ns_options.h"
#include <string>
#include <unistd.h>

typedef int fd_t;

class ns {
public:
    [[nodiscard]] const std::string &get_name(){
        return m_name;
    }
    [[nodiscard]] fd_t get_fd() const{
        return m_fd;
    }

    ns() = delete;

    virtual void setup_ns(const ns_options &opts) = 0;

    virtual void configure_ns(const ns_options &opts) = 0;

    virtual void init_internal();

    virtual void init_external();

    ~ns();

private:
    std::string m_name;
    fd_t m_fd;
};


#endif //JOCKER_SERVER_NS_H
