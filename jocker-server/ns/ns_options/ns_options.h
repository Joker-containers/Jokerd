#ifndef JOCKER_SERVER_NS_OPTIONS_H
#define JOCKER_SERVER_NS_OPTIONS_H

#include "ns_types/ns_type.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

class ns_options {
public:
    ns_options();

    explicit ns_options(const std::vector<std::string> &opts);

    explicit ns_options(const std::vector<std::string>& ns_collection, const std::vector<bool>& entry_valid);

    [[nodiscard]] const std::vector<std::string> &get_required_ns() const {
        return m_ns_collection;
    }

    [[nodiscard]] const std::vector<bool> &get_ns_mask() const {
        return m_entry_valid;
    }

    void add_namespace(ns_type tp, std::string &ns_name);

private:
    std::vector<std::string> m_ns_collection;
    std::vector<bool> m_entry_valid;
};

#endif //JOCKER_SERVER_NS_OPTIONS_H