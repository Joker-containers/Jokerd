#ifndef JOCKER_SERVER_NS_H
#define JOCKER_SERVER_NS_H

#include "ns_options/ns_options.h"
#include "ns_options/ns_conf_repository.h"
#include <string>
#include <unistd.h>
#include <utility>

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

    ns(std::string name, int fd, ns_type tp, pid_t process_pid);

    void add_pid(pid_t pid);

    virtual void setup_ns(const ns_conf_repository &opts) = 0;

    virtual void configure_ns(const ns_conf_repository &opts) = 0;

    virtual void init_internal(const ns_conf_repository &opts);

    virtual void init_external(const ns_conf_repository &opts);

    ~ns();

protected:
    ns_type m_tp;
    std::string m_name;
    fd_t m_fd;
    std::vector<pid_t> m_processes_inside;
};


#endif //JOCKER_SERVER_NS_H
