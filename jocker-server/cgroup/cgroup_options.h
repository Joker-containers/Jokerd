#ifndef JOCKER_SERVER_CGROUP_OPTIONS_H
#define JOCKER_SERVER_CGROUP_OPTIONS_H
#include <string>

class cgroup_options {
public:
    std::string read_bps;
    std::string write_bps;
    std::string cfs_quota;
    std::string cfs_period;
    std::string memory_high; // try to limit mem usage
    std::string memory_max; // kill process if over
    std::string swap_max; // rest will not be swapped
    std::string pids_max;
};


#endif //JOCKER_SERVER_CGROUP_OPTIONS_H
