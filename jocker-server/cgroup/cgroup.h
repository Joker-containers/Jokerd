#ifndef JOCKER_SERVER_CGROUP_OPTIONS_H
#define JOCKER_SERVER_CGROUP_OPTIONS_H
#include <iostream>

class cgroup {
public:
    std::string cgroup_name;
    std::string read_bps;
    std::string write_bps;
    std::string cfs_quota;
    std::string cfs_period;
    std::string memory_high; // try to limit mem usage
    std::string memory_max; // kill process if over
    std::string swap_max; // rest will not be swapped
    std::string pids_max;

    void create_cgroup(std::string& cpu_io_controller, std::string& mem_pid_controller) const;

    static bool write_to_file(const std::string& filePath, const std::string& value);
    void set_cpu_limit(std::string& cpu_io_controller) const;
    void set_memory_limit(std::string& mem_pid_controller) const;
    void set_io_limit(std::string& cpu_io_controller) const;
    void set_pids_limit(std::string& mem_pid_controller) const;

    void add_child_to_cgroup(std::string& cpu_io_controller, std::string& mem_pid_controller, int pid) const;
};


#endif //JOCKER_SERVER_CGROUP_OPTIONS_H
