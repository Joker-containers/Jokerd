#include "ns_options.h"

ns_options::ns_options() {
    _ns_collection.resize(NS_TYPES_NUM);
    _entry_valid.resize(NS_TYPES_NUM, false);
}

/* Constructs object from a vector of namespace's names.
 * Empty string means default namespace (and absence of corresponding option) */
ns_options::ns_options(const std::vector<std::string> &opts) {
    if (opts.size() != NS_TYPES_NUM){
        throw std::runtime_error("Invalid argument is given to generate the list of options for namespaces");
    }
    _ns_collection.reserve(NS_TYPES_NUM);
    _entry_valid.reserve(NS_TYPES_NUM);
    for (const auto &entry: opts){
        if (entry.empty()){
            _ns_collection.emplace_back();
            _entry_valid.emplace_back(false);
        }
        else {
            _ns_collection.emplace_back(entry);
            _entry_valid.emplace_back(true);
        }
    }
}

ns_options::ns_options(const std::vector<std::string> &ns_collection, const std::vector<bool> &entry_valid) {
    if (ns_collection.size() != NS_TYPES_NUM || entry_valid.size() != NS_TYPES_NUM){
        throw std::runtime_error("Attempts to construct namespace options with wrong arguments");
    }
    _ns_collection = ns_collection;
    _entry_valid = entry_valid;
}

void ns_options::add_namespace(ns_type tp, std::string &ns_name) {
    if (tp >= NS_TYPES_NUM){
        throw std::runtime_error("An attempt to add a name of the non-existing namespace type occured");
    }
    if (_entry_valid[tp]){
        throw std::runtime_error("A namespace of such a type exists already!");
    }
    _ns_collection[tp] = ns_name;
    _entry_valid[tp] = true;
}
