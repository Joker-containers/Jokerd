#include "user_ns.h"

user_ns::user_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void user_ns::setup_ns(const ns_options &opts) {

}

void user_ns::configure_ns(const ns_options &opts) {

}
