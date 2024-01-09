#include "net_ns_config.h"

net_ns_config::net_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}
