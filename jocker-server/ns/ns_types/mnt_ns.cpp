#include "mnt_ns.h"
#include <sys/mount.h>
#include <sys/stat.h>

mnt_ns::mnt_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void mnt_ns::external_setup_ns(const ns_conf_repository &opts) {

}

void mnt_ns::internal_setup_ns(const ns_conf_repository &opts) {
    const auto &my_conf = opts.mnt_ns_configs.at(m_name);
    const char *mnt = my_conf.new_rootfs_path.c_str();
    syscall_wrapper(mount, "mount", my_conf.new_rootfs_path.c_str(), mnt, "ext4", MS_BIND, "");
    syscall_wrapper(chdir, "chdir", mnt);
    if (mkdir(my_conf.put_old.c_str(), 0777) && errno != EEXIST){
        throw std::runtime_error("Failed to mkdir for putting old filesystem there");
    }

    syscall_wrapper(syscall, "syscall", SYS_pivot_root, ".", my_conf.put_old.c_str());

    syscall_wrapper(chdir, "chdir", "/");
}

void mnt_ns::init_internal(const ns_conf_repository &opts) {
    const auto &my_conf = opts.mnt_ns_configs.at(m_name);
    syscall_wrapper(umount2, "umount2", my_conf.put_old.c_str(), MNT_DETACH);
}

void mnt_ns::configure_ns(const ns_conf_repository &opts) {

}