#include "cgroup_manager.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/mount.h>


// Mount controllers against cgroup fs
void cgroup_manager::setup_controllers() {
    if (mkdir(cpu_and_io_path.c_str(), 0755) == -1) {
        if (errno != EEXIST) {
            throw std::runtime_error("Failed to create cpu and io controllers dir.");
        }
    } else {
        std::string command = "mount -t cgroup -o cpu,cpuacct,blkio none " + cpu_and_io_path;
        if (system(command.c_str()) == 0) {
            std::cout << "Cpu and IO controllers mounted." << std::endl;
        } else {
            throw std::runtime_error("Failed to mount cpu and io controllers dir.");
        }
    }

    if (mkdir(mem_and_pid_path.c_str(), 0755) == -1 && errno != EEXIST) {
        if (errno != EEXIST) {
            throw std::runtime_error("Failed to create mem and pid controllers dir.");
        }
    } else {
        std::string command = "mount -t cgroup2 none " + mem_and_pid_path;
        if (system(command.c_str()) == 0) {
            std::cout << "Memory and pids controllers mounted." << std::endl;
        } else {
            throw std::runtime_error("Failed to mount memory and pids controllers dir.");
        }
    }
}

void cgroup_manager::child_remount_cgroup() {
    // Create a new cgroup namespace
    if (unshare(CLONE_NEWCGROUP) != 0) {
        throw std::runtime_error("Failed to create new cgroup namespace.");
    }

    // Mount the cgroup filesystem to the container's root
    if (umount(cpu_and_io_path.c_str()) != 0){
        throw std::runtime_error("Failed to unmount cgroup.");
    }
    if (umount(mem_and_pid_path.c_str()) != 0){
        throw std::runtime_error("Failed to unmount cgroup.");
    }

    setup_controllers();
}

void cgroup_manager::init_cgroup(const cgroup& cgroup_obj){
    if (!cgroups.contains(cgroup_obj.cgroup_name)) {
        cgroup_obj.create_cgroup(cpu_and_io_path, mem_and_pid_path);

        cgroup_obj.set_cpu_limit(cpu_and_io_path);
        cgroup_obj.set_io_limit(cpu_and_io_path);
        cgroup_obj.set_memory_limit(mem_and_pid_path);
        cgroup_obj.set_pids_limit(mem_and_pid_path);

        cgroups.insert(cgroup_obj.cgroup_name);
    }
}