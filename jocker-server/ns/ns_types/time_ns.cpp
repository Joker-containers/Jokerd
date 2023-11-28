#include "time_ns.h"

time_ns::time_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void time_ns::setup_ns(const ns_options &opts) {

}

void time_ns::configure_ns(const ns_options &opts) {

}
