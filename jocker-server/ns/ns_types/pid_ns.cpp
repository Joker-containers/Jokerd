#include "pid_ns.h"

pid_ns::pid_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void pid_ns::setup_ns(const ns_options &opts) {

}

void pid_ns::configure_ns(const ns_options &opts) {

}
