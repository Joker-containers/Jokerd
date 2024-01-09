#include "ipc_ns.h"

ipc_ns::ipc_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void ipc_ns::external_setup_ns() {

}

void ipc_ns::configure_ns() {

}

ipc_ns::ipc_ns(std::string name): ns(std::move(name)){

}
