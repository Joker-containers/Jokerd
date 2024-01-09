#include "user_ns.h"

#include <utility>

user_ns::user_ns(std::string name, int fd, pid_t process_pid): ns(std::move(name), fd, process_pid) {}

void user_ns::external_setup_ns() {
    if (!active){
        pid_t pid;
        if (m_processes_inside.empty()){
            throw std::runtime_error("No process is binded to the namespace just created");
        }
        else{
            pid = m_processes_inside[0];
        }

        std::stringstream path, line;

        // Creating uid mappings
        path << "/proc/" << pid << "/uid_map";
        if (uid_mappings.empty()){
            line << "0 1000 1\n";
            line << "1 1 999\n";
            line << "1001 1001 4294966294\n";

        }
        else{
            for (const auto &conf_line: uid_mappings){
                line << conf_line << "\n";
            }
        }
        write_to_file(path.str(), line.str());
        path.str(std::string()), line.str(std::string());

        // Fix of the CVE-2014-8989 vulnerability
        path << "/proc/" << pid << "/setgroups";
        line << "deny";
        write_to_file(path.str(), line.str());
        path.str(std::string()), line.str(std::string());

        auto gid = getgid();

        // Creating gid mappings
        path << "/proc/" << pid << "/gid_map";

        if (gid_mappings.empty()){
            line << "0 " << gid << " 1\n";
        }
        else{
            for (const auto &conf_line: gid_mappings){
                line << conf_line << "\n";
            }
        }
        write_to_file(path.str(), line.str());
    }
}

void user_ns::internal_setup_ns() {


}

void user_ns::init_external() {

}

void user_ns::init_internal() {
    if (!active){
        syscall_wrapper(setuid, "setuid", 0);
        syscall_wrapper(setgid, "setgid", 0);
    }
}


void user_ns::configure_ns() {

}

user_ns::user_ns(std::string name, std::vector<std::string> uid_map, std::vector<std::string> gid_map): ns(std::move(name)), uid_mappings(std::move(uid_map)), gid_mappings(std::move(gid_map)) {

}
