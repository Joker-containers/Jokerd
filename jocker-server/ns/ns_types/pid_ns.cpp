#include <sys/stat.h>
#include <sys/mount.h>
#include "pid_ns.h"

pid_ns::pid_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void pid_ns::external_setup_ns() {

}

void pid_ns::configure_ns() {

}

void pid_ns::internal_setup_ns() {
    if (!active){
        // It's important that this code should be executed after setup of the mount
        // namespace, but before its init.
        if (mkdir("/proc", 0555) && errno != EEXIST)
            throw std::runtime_error("Failed to mkdir /proc\n");

        syscall_wrapper(mount, "mount", "proc", "/proc", "proc", 0, "");
    }
}

void pid_ns::init_internal() {

}

void pid_ns::init_external() {

}

pid_ns::pid_ns(std::string name): ns(std::move(name)) {

}
