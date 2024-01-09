#include "time_ns_config.h"
#include "ns_types/time_ns.h"

time_ns_config::time_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}

std::shared_ptr<ns> time_ns_config::create_ns(const std::string &name) {
    return std::make_shared<time_ns>(name);
}
