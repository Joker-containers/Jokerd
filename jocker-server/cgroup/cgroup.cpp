#include "cgroup.h"
#include <iostream>
#include <sys/stat.h>

void cgroup::create_cgroup(std::string& cpu_io_controller, std::string& mem_pid_controller) const{
    std::string cpu_io_cgroup_path = cpu_io_controller + cgroup_name;
    std::string mem_pid_cgroup_path = mem_pid_controller + cgroup_name;

    if (mkdir(cpu_io_cgroup_path.c_str(), 0755) == -1 && errno != EEXIST) {
        throw std::runtime_error("Failed to create first cgroup directory.");
    } else {
        std::cout<<"Cgroup1 " << cgroup_name << " created." << std::endl;
    }

    if (mkdir(mem_pid_cgroup_path.c_str(), 0755) == -1 && errno != EEXIST) {
        throw std::runtime_error("Failed to create second cgroup directory.");
    } else {
        std::cout<<"Cgroup2 " << cgroup_name << " created." << std::endl;
    }
}

bool cgroup::write_to_file(const std::string& filePath, const std::string& value) {
    auto command = "echo " + value + " > " + filePath;
    if (system(command.c_str()) == 0){
        return true;
    } else {
        return false;
    }
}

void cgroup::set_cpu_limit(std::string& cpu_io_controller) const {
    std::string cgroup_path = cpu_io_controller + cgroup_name;

    if (!cfs_quota.empty()) {
        if (!write_to_file(cgroup_path + "/cpu.cfs_quota_us", cfs_quota)) {
            throw std::runtime_error("Failed to set cpu limit.");
        }
    }

    if (!cfs_period.empty()) {
        if (!write_to_file(cgroup_path + "/cpu.cfs_period_us", cfs_period)) {
            throw std::runtime_error("Failed to set cpu limit.");
        }

        std::cout << "CPU limit set." << std::endl;
    }
}

void cgroup::set_memory_limit(std::string& mem_pid_controller) const {
    std::string cgroup_path = mem_pid_controller + cgroup_name;

    if (!memory_high.empty()) {
        if (!write_to_file(cgroup_path + "/memory.high", memory_high)) {
            throw std::runtime_error("Failed to set memory high limit.");
        }
        std::cout<<"High memory limit set."<<std::endl;
    }

    if (!memory_max.empty()) {
        if (!write_to_file(cgroup_path + "/memory.max", memory_max)) {
            throw std::runtime_error("Failed to set memory max limit.");
        }
        std::cout<<"Max memory limit set."<<std::endl;
    }

    if (!swap_max.empty()) {
        if (!write_to_file(cgroup_path + "/memory.swap.max", swap_max)) {
            throw std::runtime_error("Failed to set memory swap max limit.");
        }
        std::cout<<"Swap memory limit set."<<std::endl;
    }
}

void cgroup::set_io_limit(std::string& cpu_io_controller) const {
    std::string cgroup_path = cpu_io_controller + cgroup_name;

    if (!read_bps.empty()) {
        if (!write_to_file(cgroup_path + "/blkio.throttle.read_bps_device", read_bps)) {
            throw std::runtime_error("Failed to set blkio read limit.");
        }
        std::cout << "Read IO limit set" << std::endl;
    }

    if (!write_bps.empty()) {
        if (!write_to_file(cgroup_path + "/blkio.throttle.write_bps_device", write_bps)) {
            throw std::runtime_error("Failed to set blkio write limit.");
        }
        std::cout << "Write IO limit set" << std::endl;
    }
}

void cgroup::set_pids_limit(std::string& mem_pid_controller) const {
    std::string cgroup_path = mem_pid_controller + cgroup_name;

    if (!pids_max.empty()) {
        if (!write_to_file(cgroup_path + "/pids.max", pids_max)) {
            throw std::runtime_error("Failed to set pids limit.");
        }
        std::cout<<"Pids limit set."<<std::endl;
    }
}

void cgroup::add_child_to_cgroup(std::string& cpu_io_controller, std::string& mem_pid_controller, int pid) const {
    if (!write_to_file(cpu_io_controller + cgroup_name + "/tasks", std::to_string(pid))) {
        throw std::runtime_error("Failed to add process to cgroup1.");
    }

    if (!write_to_file(mem_pid_controller + cgroup_name + "/cgroup.procs", std::to_string(pid))) {
        throw std::runtime_error("Failed to add process to cgroup2.");
    }

    std::cout << "Process's successfully added to tasks." << std::endl;
}