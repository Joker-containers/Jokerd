#include "pid_ns.h"

pid_ns::pid_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void pid_ns::setup_ns(const ns_conf_repository &opts) {

}

void pid_ns::configure_ns(const ns_conf_repository &opts) {

}
