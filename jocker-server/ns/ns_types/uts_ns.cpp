#include "uts_ns.h"

#include <iostream>

uts_ns::uts_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void uts_ns::configure_ns(const ns_conf_repository &opts) {

}

void uts_ns::internal_setup_ns(const ns_conf_repository &opts) {
    const auto &conf = opts.uts_ns_configs.at(m_name);

    struct utsname uts{};

    /* Change hostname in UTS namespace of child. */
    syscall_wrapper(sethostname, "sethostname", conf.hostname.c_str(), conf.hostname.size());

    /* Retrieve and display hostname. */
    syscall_wrapper(uname, "uname", &uts);

    std::cout << "Child host machine name: " << uts.nodename << std::endl;
}

void uts_ns::init_internal(const ns_conf_repository &opts) {

}
