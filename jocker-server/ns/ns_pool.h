#ifndef JOCKER_SERVER_NS_POOL_H
#define JOCKER_SERVER_NS_POOL_H

#include "ns_types/ipc_ns.h"
#include "ns_types/net_ns.h"
#include "ns_types/mnt_ns.h"
#include "ns_types/pid_ns.h"
#include "ns_types/time_ns.h"
#include "ns_types/user_ns.h"
#include "ns_types/uts_ns.h"
#include "ns_types/ns_type.h"
#include "ns_group.h"
#include "ns_options/ns_options.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

class ns_pool {
public:
    ns_pool();

    [[nodiscard]] bool exists_ns(ns_type tp, const std::string &ns_name) const;

    void declare_ns(ns_type tp, const std::shared_ptr<ns> &to_declare);

    [[nodiscard]] std::shared_ptr<ns> get_ns(ns_type tp, const std::string &ns_name) const;

    bool delete_ns(ns_type tp, const std::string &ns_name);

    ns_group get_ns_group(ns_options &opts) const;

private:
    std::vector<std::vector<std::shared_ptr<ns>>> all_ns;
};


#endif //JOCKER_SERVER_NS_POOL_H
