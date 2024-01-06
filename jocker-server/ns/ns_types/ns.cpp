#include "ns.h"


void ns::setup_ns(const ns_conf_repository &opts) {
    // Place for some initializations etc

    configure_ns(opts);
}

/* A function for namespace initialization from the inside of this namespace i.e. it's called from the
 * cloned (child) process */
void ns::init_internal(const ns_conf_repository &opts) {

}

/* A function for namespace initialization from the outside of this namespace i.e. it's called from the
 * parent daemon (likely superuser) process */
void ns::init_external(const ns_conf_repository &opts) {

}

ns::~ns() {
    close(m_fd);
}

ns::ns(std::string name, int fd, ns_type tp, pid_t process_pid): m_name(std::move(name)), m_fd(fd), m_processes_inside(process_pid), m_tp(tp){

}

void ns::add_pid(pid_t pid) {
    m_processes_inside.emplace_back(pid); // TODO: use this
}
