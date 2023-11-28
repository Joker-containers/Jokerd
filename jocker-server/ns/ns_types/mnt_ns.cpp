#include "mnt_ns.h"

mnt_ns::mnt_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void mnt_ns::setup_ns(const ns_options &opts) {

}

void mnt_ns::configure_ns(const ns_options &opts) {

}