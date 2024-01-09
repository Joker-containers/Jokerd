#ifndef JOCKER_SERVER_MNT_NS_H
#define JOCKER_SERVER_MNT_NS_H


#include "ns.h"

class mnt_ns: public ns {
public:
    mnt_ns() = delete;

    mnt_ns(std::string name, std::string new_rootfs,
           std::string filesystem_tp, std::string put_old);

    mnt_ns(std::string name, int fd, pid_t process_pid);

    void external_setup_ns() override;

    void internal_setup_ns() override;

    void init_internal() override;

    void configure_ns() override;

    [[nodiscard]] int get_type() const override{
        return MOUNT;
    }

private:
    std::string new_rootfs_path{};
    std::string filesystem = "ext4";
    std::string put_old = ".put_old";
};


#endif //JOCKER_SERVER_MNT_NS_H
