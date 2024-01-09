#include "uts_ns.h"

#include <iostream>

uts_ns::uts_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void uts_ns::configure_ns() {

}

void uts_ns::internal_setup_ns() {
    if (!active){
        struct utsname uts{};

        /* Change hostname in UTS namespace of child. */
        syscall_wrapper(sethostname, "sethostname", hostname.c_str(), hostname.size());

        /* Retrieve and display hostname. */
        syscall_wrapper(uname, "uname", &uts);

        std::cout << "Child host machine name: " << uts.nodename << std::endl;
    }
}

void uts_ns::init_internal() {

}

uts_ns::uts_ns(std::string name, std::string hostname): ns(std::move(name)), hostname(std::move(hostname)) {

}
