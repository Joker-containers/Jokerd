#include "ipc_ns.h"

ipc_ns::ipc_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void ipc_ns::external_setup_ns(const ns_conf_repository &opts) {

}

void ipc_ns::configure_ns(const ns_conf_repository &opts) {

}