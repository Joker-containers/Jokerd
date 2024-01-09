#ifndef JOCKER_SERVER_MNT_NS_CONFIG_H
#define JOCKER_SERVER_MNT_NS_CONFIG_H

#include "common.h"
#include "ns_config.h"
#include <string>

class mnt_ns_config: public ns_config {
public:
    explicit mnt_ns_config(std::ifstream &file, int id);

    std::shared_ptr<ns> create_ns() override;

    std::string new_rootfs_path{};
    std::string filesystem = "ext4";
    std::string put_old = ".put_old";
};


#endif //JOCKER_SERVER_MNT_NS_CONFIG_H
