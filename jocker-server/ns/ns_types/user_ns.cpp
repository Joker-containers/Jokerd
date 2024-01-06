#include "user_ns.h"

user_ns::user_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void user_ns::setup_ns(const ns_conf_repository &opts) {

}

void user_ns::configure_ns(const ns_conf_repository &opts) {

}
