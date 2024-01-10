#include "mnt_ns.h"
#include <sys/mount.h>
#include <sys/stat.h>

#include <utility>

mnt_ns::mnt_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void mnt_ns::external_setup_ns() {

}

void mnt_ns::internal_setup_ns() {
    if (!active){
        const char *mnt = new_rootfs_path.c_str();
        syscall_wrapper(mount, "mount", new_rootfs_path.c_str(), mnt, "ext4", MS_BIND, "");
        syscall_wrapper(chdir, "chdir", mnt);
        //std::cerr << put_old << std::endl;
        if (mkdir(put_old.c_str(), 0777) && errno != EEXIST){
            perror("mkdir");
            throw std::runtime_error("Failed to mkdir for putting old filesystem there");
        }

        std::cerr << "test" << std::endl;
        syscall_wrapper(syscall, "syscall", SYS_pivot_root, ".", put_old.c_str());
        std::cerr << "test1" << std::endl;

        syscall_wrapper(chdir, "chdir", "/");
    }
}

void mnt_ns::init_internal() {
    if (!active){
        syscall_wrapper(umount2, "umount2", put_old.c_str(), MNT_DETACH);
    }
}

void mnt_ns::configure_ns() {

}

mnt_ns::mnt_ns(std::string name, std::string new_rootfs, std::string filesystem_tp, std::string put_old):
ns(std::move(name)), new_rootfs_path(std::move(new_rootfs)), filesystem(std::move(filesystem_tp)), put_old(std::move(put_old)){

}
