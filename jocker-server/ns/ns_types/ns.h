#ifndef JOCKER_SERVER_NS_H
#define JOCKER_SERVER_NS_H

#include "syscall_wrapper.h"
#include "ns_type.h"
#include "common.h"
#include <string>
#include <unistd.h>
#include <utility>

typedef int fd_t;

constexpr int MOCK_DESCRIPTOR = -1;

class ns {
public:
    [[nodiscard]] const std::string &get_name() const{
        return m_name;
    }
    [[nodiscard]] fd_t get_fd() const{
        return m_fd;
    }

    ns() = delete;

    explicit ns(std::string &name);

    explicit ns(std::string &&name);

    ns(std::string name, int fd, pid_t process_pid);

    void add_pid(pid_t pid);

    virtual void external_setup_ns();

    virtual void internal_setup_ns();

    virtual void configure_ns() = 0;

    virtual void init_internal();

    virtual void init_external();

    [[nodiscard]] bool is_active() const {
        return active;
    }

    void set_active() {
        active = true;
    }

    [[nodiscard]] virtual int get_type() const = 0;

    void set_fd(int fd);

    ~ns();

    std::string m_name;
protected:
    bool active = false;
    fd_t m_fd = MOCK_DESCRIPTOR;
    std::vector<pid_t> m_processes_inside = {}; // TODO
};

std::shared_ptr<ns> create_namespace_entry(ns_type tp, const std::string &ns_name, int fd, pid_t pid);

std::string get_ns_handle_path(ns_type tp, pid_t pid);

int get_ns_handle(ns_type tp, pid_t pid);


#endif //JOCKER_SERVER_NS_H
