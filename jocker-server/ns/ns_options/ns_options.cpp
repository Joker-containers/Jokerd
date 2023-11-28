#include "ns_options.h"

ns_options::ns_options() {
    m_ns_collection.reserve(NS_TYPES_NUM);
    m_entry_valid.reserve(NS_TYPES_NUM);
    for (size_t i = 0; i < NS_TYPES_NUM; i++){
        m_ns_collection.emplace_back();
        m_entry_valid.emplace_back(false);
    }
}

/* Constructs object from a vector of namespace's names.
 * Empty string means default namespace (and absence of corresponding option) */
ns_options::ns_options(const std::vector<std::string> &opts) {
    if (opts.size() != NS_TYPES_NUM){
        throw std::runtime_error("Invalid argument is given to generate the list of options for namespaces");
    }
    m_ns_collection.reserve(NS_TYPES_NUM);
    m_entry_valid.reserve(NS_TYPES_NUM);
    for (const auto &entry: opts){
        if (entry.empty()){
            m_ns_collection.emplace_back();
            m_entry_valid.emplace_back(false);
        }
        else {
            m_ns_collection.emplace_back(entry);
            m_entry_valid.emplace_back(true);
        }
    }
}

ns_options::ns_options(const std::vector<std::string> &ns_collection, const std::vector<bool> &entry_valid) : m_ns_collection(ns_collection), m_entry_valid(entry_valid) {

}

void ns_options::add_namespace(ns_type tp, std::string &ns_name) {
    assert(tp < NS_TYPES_NUM);
    m_ns_collection[tp] = ns_name;
    m_entry_valid[tp] = true;
}
