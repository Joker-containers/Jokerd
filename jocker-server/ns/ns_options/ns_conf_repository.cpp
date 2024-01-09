#include "ns_conf_repository.h"

ns_conf_repository::ns_conf_repository() {
    all_configs.resize(NS_TYPES_NUM);
}

void ns_conf_repository::add_config(int ns_type, std::shared_ptr<ns_config> &&conf) {
    if (ns_type >= NS_TYPES_NUM){
        throw std::runtime_error("Bad namespace type!");
    }

    all_configs[ns_type].emplace_back(conf);
}

std::shared_ptr<ns_config> ns_conf_repository::find_config(int ns_type, int id) {
    if (ns_type >= NS_TYPES_NUM){
        throw std::runtime_error("Bad namespace type!");
    }
    const auto &needed_ns_collection = all_configs[ns_type];
    auto it = std::find_if(needed_ns_collection.begin(), needed_ns_collection.end(),
                 [id](const std::shared_ptr<ns_config> &arg){return arg->compare_id(id);});
    return *it;
}
