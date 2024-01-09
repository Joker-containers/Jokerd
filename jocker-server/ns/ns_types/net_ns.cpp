#include "net_ns.h"

net_ns::net_ns(std::string name): ns(std::move(name)) {}

net_ns::net_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void net_ns::external_setup_ns() {

}

void net_ns::configure_ns() {

}
