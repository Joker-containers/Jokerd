#include "ns_pool.h"



std::shared_ptr<ns> ns_pool::get_ns(ns_type tp, const std::string &ns_name){
    std::shared_ptr<ns> result = nullptr; // In case namespace with such a name is not found, function returns nullptr
    const auto it = ns_type_map.find(tp);
    if (it != ns_type_map.end()) {
        const auto &tp_ns_collection = it->second;
        for (const auto &ns_entry: tp_ns_collection){
            if (ns_entry->get_name() == ns_name){
                result = ns_entry;
            }
        }
    } else {
        throw std::runtime_error("Unexpected namespace type queried from ns_pool!");
    }
    return result;
}

void ns_pool::register_ns(ns_type tp, const std::shared_ptr<ns>& to_register) {
    const auto it = ns_type_map.find(tp);
    if (it != ns_type_map.end()) {
        auto &tp_ns_collection = it->second;
        tp_ns_collection.emplace_back(to_register);
    } else {
        throw std::runtime_error("Unexpected namespace type queried from ns_pool!");
    }
}

bool ns_pool::delete_ns(ns_type tp, const std::string &ns_name) {
    const auto map_it = ns_type_map.find(tp);
    if (map_it != ns_type_map.end()) {
        auto &tp_ns_collection = map_it->second;
        for (auto it = begin(tp_ns_collection); it != end(tp_ns_collection); ++it) {
            if (it->get()->get_name() == ns_name){
                tp_ns_collection.erase(it);
                return true;
            }
        }
    } else {
        throw std::runtime_error("Unexpected namespace type queried from ns_pool!");
    }
    return false;
}

/* Checks if the namespace is registered in this pool */
bool ns_pool::exists_ns(ns_type tp, const std::string &ns_name) {
    const auto it = ns_type_map.find(tp);
    if (it != ns_type_map.end()) {
        const auto &tp_ns_collection = it->second;
        return std::any_of(tp_ns_collection.begin(), tp_ns_collection.end(), [&ns_name](const auto &ns_entry) {
                                      return ns_entry->get_name() == ns_name;
                                  });

    } else {
        throw std::runtime_error("Unexpected namespace type queried from ns_pool!");
    }
}
