#ifndef JOCKER_SERVER_NS_GROUP_H
#define JOCKER_SERVER_NS_GROUP_H

#include <ns_types/ns.h>
#include <vector>
#include <string>
#include <memory>

class ns_group {
public:
    ns_group();

    explicit ns_group(std::vector<std::shared_ptr<ns>> &namespaces);

    void add_ns(ns_type tp, std::shared_ptr<ns> &to_add);

    void add_ns(ns_type tp, std::shared_ptr<ns> &&to_add);

    [[nodiscard]] const std::vector<std::shared_ptr<ns>> &get_required_ns() const{
        return m_ns_collection;
    }

    [[nodiscard]] const std::vector<bool> &get_ns_mask() const{
        return m_entry_valid;
    }
private:
    // TBD: info about each of 8? namespaces: mnt, pid, net, ipc, uts, user, cgroup?, time
    // Likely each namespace will have own class.
    std::vector<std::shared_ptr<ns>> m_ns_collection;
    std::vector<bool> m_entry_valid;
};


#endif //JOCKER_SERVER_NS_GROUP_H
