#ifndef JOCKER_SERVER_CGROUP_H
#define JOCKER_SERVER_CGROUP_H

#include <cgroup.h>
#include <set>

class cgroup_manager {
public:
    void setup_controllers();
    void child_remount_cgroup();

    void init_cgroup(const cgroup& cgroup_obj);
private:
    // Path to the cgroup folder
    std::string cpu_and_io_path{"/sys/fs/cgroup/cpu,blkio/"};
    std::string mem_and_pid_path{"/sys/fs/cgroup/memory,pids/"};
    std::set<std::string> cgroups;
};

#endif //JOCKER_SERVER_CGROUP_H
