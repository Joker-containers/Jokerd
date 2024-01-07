#include "time_ns.h"

time_ns::time_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void time_ns::external_setup_ns(const ns_conf_repository &opts) {

}

void time_ns::configure_ns(const ns_conf_repository &opts) {

}
