#include "ipc_ns.h"

ipc_ns::ipc_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void ipc_ns::setup_ns(const ns_options &opts) {

}

void ipc_ns::configure_ns(const ns_options &opts) {

}