#include "ns.h"
#include "ipc_ns.h"
#include "mnt_ns.h"
#include "net_ns.h"
#include "pid_ns.h"
#include "time_ns.h"
#include "user_ns.h"
#include "uts_ns.h"
#include "common.h"


void ns::external_setup_ns(const ns_conf_repository &opts) {
    // Place for some initializations etc

    configure_ns(opts);
}

void ns::internal_setup_ns(const ns_conf_repository &opts) {
    // Place for some initializations etc

    configure_ns(opts);
}

/* A function for namespace initialization from the inside of this namespace i.e. it's called from the
 * cloned (child) process */
void ns::init_internal(const ns_conf_repository &opts) {

}

/* A function for namespace initialization from the outside of this namespace i.e. it's called from the
 * parent Daemon (likely superuser) process */
void ns::init_external(const ns_conf_repository &opts) {

}

ns::~ns() {
    close(m_fd);
}

ns::ns(std::string name, int fd, ns_type tp, pid_t process_pid): m_name(std::move(name)), m_fd(fd), m_processes_inside(), m_tp(tp){
    m_processes_inside.emplace_back(process_pid);
}

void ns::add_pid(pid_t pid) {
    m_processes_inside.emplace_back(pid); // TODO: use this
}

std::shared_ptr<ns> create_namespace_entry(ns_type tp, const std::string &ns_name, int fd, pid_t pid){
    std::shared_ptr<ns> new_ns;
    switch (tp) {
        case IPC:
            new_ns = std::make_shared<ipc_ns>(ns_name, fd, tp, pid);
            break;
        case NETWORK:
            new_ns = std::make_shared<net_ns>(ns_name, fd, tp, pid);
            break;
        case MOUNT:
            new_ns = std::make_shared<mnt_ns>(ns_name, fd, tp, pid);
            break;
        case PID:
            new_ns = std::make_shared<pid_ns>(ns_name, fd, tp, pid);
            break;
        case TIME:
            new_ns = std::make_shared<time_ns>(ns_name, fd, tp, pid);
            break;
        case USER:
            new_ns = std::make_shared<user_ns>(ns_name, fd, tp, pid);
            break;
        case UTS:
            new_ns = std::make_shared<uts_ns>(ns_name, fd, tp, pid);
            break;
        default:
            throw std::runtime_error("Unsupported namespace type!");
    }
    return new_ns;
}

std::string get_ns_handle_path(ns_type tp, pid_t pid){
    std::stringstream path;
    path << "/proc/" << pid << "/ns/" << NS_SHORTCUT.at(tp);
    return path.str();
}

int get_ns_handle(ns_type tp, pid_t pid){
    auto path = get_ns_handle_path(tp, pid);
    int fd = syscall_wrapper(open, "open", path.c_str(), O_RDONLY);
    return fd;
}