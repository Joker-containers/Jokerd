#include "uts_ns.h"

uts_ns::uts_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void uts_ns::configure_ns(const ns_options &opts) {

}

void uts_ns::setup_ns(const ns_options &opts) {

}
