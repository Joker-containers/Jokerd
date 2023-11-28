#include "ns_group.h"

ns_group::ns_group() {
    m_ns_collection.reserve(NS_TYPES_NUM);
    m_entry_valid.reserve(NS_TYPES_NUM);
    for (size_t i = 0; i < NS_TYPES_NUM; i++){
        m_ns_collection.emplace_back(nullptr);
        m_entry_valid.emplace_back(false);
    }
}

ns_group::ns_group(std::vector<std::shared_ptr<ns>> &namespaces) {
    if (namespaces.size() != NS_TYPES_NUM){
        throw std::runtime_error("Attempt to construct the ns_group using an inappropriate argument format!");
    }
    m_ns_collection = namespaces;
    throw std::runtime_error("Not implemented yet!");
}

void ns_group::add_ns(ns_type tp, std::shared_ptr<ns> &to_add) {
    if (tp >= m_ns_collection.size()){
        throw std::runtime_error("Attempting to add namespace of an unsupported type");
    }
    m_ns_collection[tp] = to_add;
    m_entry_valid[tp] = true;
}

void ns_group::add_ns(ns_type tp, std::shared_ptr<ns> &&to_add) {
    if (tp >= m_ns_collection.size()){
        throw std::runtime_error("Attempting to add namespace of an unsupported type");
    }
    m_ns_collection[tp] = std::move(to_add);
    m_entry_valid[tp] = true;
}