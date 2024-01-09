#include "pid_ns_config.h"
#include "ns_types/pid_ns.h"

pid_ns_config::pid_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}

std::shared_ptr<ns> pid_ns_config::create_ns(const std::string &name) {
    return std::make_shared<pid_ns>(name);
}
