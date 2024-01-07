#include <sys/stat.h>
#include <sys/mount.h>
#include "pid_ns.h"

pid_ns::pid_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void pid_ns::external_setup_ns(const ns_conf_repository &opts) {

}

void pid_ns::configure_ns(const ns_conf_repository &opts) {

}

void pid_ns::internal_setup_ns(const ns_conf_repository &opts) {

    // It's important that this code should be executed after setup of the mount
    // namespace, but before its init.
    if (mkdir("/proc", 0555) && errno != EEXIST)
        throw std::runtime_error("Failed to mkdir /proc\n");

    syscall_wrapper(mount, "mount", "proc", "/proc", "proc", 0, "");
}

void pid_ns::init_internal(const ns_conf_repository &opts) {

}

void pid_ns::init_external(const ns_conf_repository &opts) {

}
