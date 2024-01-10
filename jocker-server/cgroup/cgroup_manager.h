#ifndef JOCKER_SERVER_CGROUP_H
#define JOCKER_SERVER_CGROUP_H

#include "cgroup.h"
#include <set>

const std::string CPU_AND_IO_PATH = "/sys/fs/cgroup/cpu,blkio/";
const std::string MEM_AND_PID_PATH = "/sys/fs/cgroup/";

class cgroup_manager {
public:
    cgroup_manager();

    static void setup_controllers();
    static void child_remount_cgroup();
    static void add_child(const std::string& cgroup_name, int pid);

    void init_cgroup(const cgroup& cgroup_obj);
private:
    std::set<std::string> cgroups;
};

#endif //JOCKER_SERVER_CGROUP_H
