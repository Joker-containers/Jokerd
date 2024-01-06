#include "mnt_ns.h"

mnt_ns::mnt_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void mnt_ns::setup_ns(const ns_conf_repository &opts) {

}

void mnt_ns::configure_ns(const ns_conf_repository &opts) {

}