#ifndef JOCKER_SERVER_NS_OPTIONS_H
#define JOCKER_SERVER_NS_OPTIONS_H

#include "ns_types/ns_type.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

/**
 * @brief A collection of namespaces the process in container should enter.
 *
 * Each process can belong only to one namespace of the given type at a time, so we
 * can describe all the namespaces needed with a vector of their names,
 * placed on the corresponding position (each name is then mapped to a namespace instance).
 * Hence, this vector should be of length equal to a quantity of the namespace types.
 *
 * However, sometimes the process is not required to enter the namespace of the specific type.
 * That is why this class also contains a boolean mask of the same length as vector,
 * which indicates if the name of the namespace in the vector of strings is relevant and should
 * be mapped to a namespace instance.
 *
 */

class ns_options {
public:
    ns_options();

    ns_options (ns_options &&) = default;

    ns_options (ns_options&) = default;

    ns_options (const ns_options&) = default;

    explicit ns_options(const std::vector<std::string> &opts);

    explicit ns_options(const std::vector<std::string>& ns_collection, const std::vector<bool>& entry_valid);

    [[nodiscard]] const std::vector<std::string> &get_required_ns() const {
        return _ns_collection;
    }

    [[nodiscard]] const std::vector<bool> &get_ns_mask() const {
        return _entry_valid;
    }

    void add_namespace(ns_type tp, std::string &ns_name);

public:
    std::vector<std::string> _ns_collection;
    std::vector<bool> _entry_valid;
};

#endif //JOCKER_SERVER_NS_OPTIONS_H