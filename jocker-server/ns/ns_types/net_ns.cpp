#include "net_ns.h"

net_ns::net_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void net_ns::setup_ns(const ns_options &opts) {

}

void net_ns::configure_ns(const ns_options &opts) {

}
