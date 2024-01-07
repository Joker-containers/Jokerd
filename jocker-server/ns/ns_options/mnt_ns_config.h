#ifndef JOCKER_SERVER_MNT_NS_CONFIG_H
#define JOCKER_SERVER_MNT_NS_CONFIG_H


#include <string>

class mnt_ns_config {
public:
    std::string new_rootfs_path{};
    std::string filesystem = "ext4";
    std::string put_old = ".put_old";
};


#endif //JOCKER_SERVER_MNT_NS_CONFIG_H
