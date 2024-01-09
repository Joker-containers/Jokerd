#include "net_ns_config.h"
#include "ns_types/net_ns.h"

net_ns_config::net_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}

std::shared_ptr<ns> net_ns_config::create_ns(const std::string &name) {
    return std::make_shared<net_ns>(name);
}
