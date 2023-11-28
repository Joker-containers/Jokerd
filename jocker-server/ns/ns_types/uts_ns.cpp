#include "uts_ns.h"

#include <iostream>

uts_ns::uts_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void uts_ns::configure_ns(const ns_conf_repository &opts) {

}

void uts_ns::setup_ns(const ns_conf_repository &opts) {

}

void uts_ns::init_external(const ns_conf_repository &opts) {
    auto conf = opts.uts_ns_configs.at(m_name);

    struct utsname uts;

    /* Change hostname in UTS namespace of child. */

    if (sethostname(conf.hostname.c_str(), conf.hostname.size()) == -1)
        err(EXIT_FAILURE, "sethostname");

    /* Retrieve and display hostname. */

    if (uname(&uts) == -1)
        err(EXIT_FAILURE, "uname");
    printf("uts.nodename in child:  %s\n", uts.nodename);
}
