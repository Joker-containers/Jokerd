#include "uts_ns_config.h"

const std::string HOSTNAME_PROP = "Hostname";

uts_ns_config::uts_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
    check_and_assign_variable(HOSTNAME_PROP, hostname, file, PARSE_ERROR_MSG);
}
