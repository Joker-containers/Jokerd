#include "user_ns_config.h"
#include "ns_types/user_ns.h"

user_ns_config::user_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}

std::shared_ptr<ns> user_ns_config::create_ns(const std::string &name) {
    return std::make_shared<user_ns>(name, uid_mappings, gid_mappings);
}


