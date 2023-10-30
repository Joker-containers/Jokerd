#ifndef JOCKER_SERVER_CGROUP_H
#define JOCKER_SERVER_CGROUP_H

#include <boost/filesystem.hpp>

class cgroup {
private:
    // Path to the cgroup folder
    boost::filesystem::path m_cgroup_path;

    // TBD: fields for 12 resource controllers
};


#endif //JOCKER_SERVER_CGROUP_H
