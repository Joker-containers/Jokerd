#include "net_ns.h"

net_ns::net_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void net_ns::external_setup_ns(const ns_conf_repository &opts) {

}

void net_ns::configure_ns(const ns_conf_repository &opts) {

}
