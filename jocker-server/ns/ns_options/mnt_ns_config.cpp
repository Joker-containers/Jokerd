#include "mnt_ns_config.h"
#include "ns_types/mnt_ns.h"

const std::string NEW_ROOTFS_PATH_PROP = "New root filesystem path";
const std::string FILESYSTEM_PROP = "Filesystem";
const std::string PUT_OLD_PROP = "Put old";

mnt_ns_config::mnt_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
    check_and_assign_variable(NEW_ROOTFS_PATH_PROP, new_rootfs_path, file, PARSE_ERROR_MSG);
    check_and_assign_variable(FILESYSTEM_PROP, filesystem, file, PARSE_ERROR_MSG);
    check_and_assign_variable(PUT_OLD_PROP, put_old, file, PARSE_ERROR_MSG);
}

std::shared_ptr<ns> mnt_ns_config::create_ns(const std::string &name) {
    return std::make_shared<mnt_ns>(name, new_rootfs_path, filesystem, put_old);
}