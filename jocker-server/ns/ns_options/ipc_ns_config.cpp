#include "ipc_ns_config.h"
#include "ns_types/ipc_ns.h"

ipc_ns_config::ipc_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}

std::shared_ptr<ns> ipc_ns_config::create_ns(const std::string &name) {
    return std::make_shared<ipc_ns>(name);
}
