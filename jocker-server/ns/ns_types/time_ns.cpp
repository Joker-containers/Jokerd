#include "time_ns.h"

time_ns::time_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void time_ns::external_setup_ns() {

}

void time_ns::configure_ns() {

}

time_ns::time_ns(std::string name): ns(std::move(name)) {

}
