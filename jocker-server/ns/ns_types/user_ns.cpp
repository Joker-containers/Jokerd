#include "user_ns.h"

user_ns::user_ns(std::string name, int fd, ns_type tp, pid_t process_pid): ns(std::move(name), fd, tp, process_pid) {}

void user_ns::external_setup_ns(const ns_conf_repository &opts) {
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
    const auto &uid_mappings = opts.user_ns_configs.at(m_name).uid_mappings;
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
    const auto &gid_mappings = opts.user_ns_configs.at(m_name).gid_mappings;
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

void user_ns::internal_setup_ns(const ns_conf_repository &opts) {


}

void user_ns::init_external(const ns_conf_repository &opts) {

}

void user_ns::init_internal(const ns_conf_repository &opts) {
    syscall_wrapper(setuid, "setuid", 0);
    syscall_wrapper(setgid, "setgid", 0);
}


void user_ns::configure_ns(const ns_conf_repository &opts) {

}
