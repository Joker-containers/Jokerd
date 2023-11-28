#include "ns.h"


void ns::setup_ns(const ns_options &opts) {
    // Place for some initializations etc

    configure_ns(opts);
}

/* A function for namespace initialization from the inside of this namespace i.e. it's called from the
 * cloned (child) process */
void ns::init_internal() {

}

/* A function for namespace initialization from the outside of this namespace i.e. it's called from the
 * parent daemon (likely superuser) process */
void ns::init_external() {

}

ns::~ns() {
    close(m_fd);
}
