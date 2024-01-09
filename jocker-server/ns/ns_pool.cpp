#include "ns_pool.h"

ns_pool::ns_pool() {
    all_ns.resize(NS_TYPES_NUM);
}

std::shared_ptr<ns> ns_pool::get_ns(ns_type tp, const std::string &ns_name) const{
    std::shared_ptr<ns> result = nullptr; // In case namespace with such a name is not found, function returns nullptr
    if (tp >= NS_TYPES_NUM){
        throw std::runtime_error("Bad namespace type requested!");
    }
    const auto &tp_ns_collection = all_ns[tp];
    for (const auto &ns_entry: tp_ns_collection){
        if (ns_entry->get_name() == ns_name){
            result = ns_entry;
            break;
        }
    }
    if (!result.get()){
        throw std::runtime_error("A non-existing namespace requested from namespace pool");
    }
    return result;
}

void ns_pool::declare_ns(ns_type tp, const std::shared_ptr<ns>& to_declare) {
    if (tp >= NS_TYPES_NUM){
        throw std::runtime_error("Bad namespace type!");
    }
    auto &tp_ns_collection = all_ns[tp];
    tp_ns_collection.emplace_back(to_declare);
}

bool ns_pool::delete_ns(ns_type tp, const std::string &ns_name) {
    if (tp >= NS_TYPES_NUM){
        throw std::runtime_error("Bad namespace type!");
    }

    auto &tp_ns_collection = all_ns[tp];
    for (auto it = begin(tp_ns_collection); it != end(tp_ns_collection); ++it) {
        if (it->get()->get_name() == ns_name){
            tp_ns_collection.erase(it);
            return true;
        }
    }

    return false;
}

/* Checks if the namespace is registered in this pool */
bool ns_pool::exists_ns(ns_type tp, const std::string &ns_name) const {
    if (tp >= NS_TYPES_NUM){
        throw std::runtime_error("Bad namespace type!");
    }

    const auto &tp_ns_collection = all_ns[tp];
    return std::any_of(tp_ns_collection.begin(), tp_ns_collection.end(), [&ns_name](const auto &ns_entry) {
                                  return ns_entry->get_name() == ns_name;
                              });
}

ns_group ns_pool::get_ns_group(ns_options &opts) const {
    auto group = ns_group();
    const auto &mask = opts.get_ns_mask();
    const auto &names = opts.get_ns_required();
    for (size_t i = 0; i < NS_TYPES_NUM; ++i){
        if (mask[i]){
            auto tp = static_cast<ns_type>(i);
            group.add_ns(tp, get_ns(tp, names[i]));
        }
    }
    return group;
}
