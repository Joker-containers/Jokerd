#ifndef JOCKER_SERVER_COMMON_H
#define JOCKER_SERVER_COMMON_H

#include "ns_types/ipc_ns.h"
#include "ns_types/net_ns.h"
#include "ns_types/mnt_ns.h"
#include "ns_types/pid_ns.h"
#include "ns_types/time_ns.h"
#include "ns_types/user_ns.h"
#include "ns_types/uts_ns.h"
#include <memory>
#include <cstring>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <iostream>

const std::unordered_map<ns_type, int> NS_CLONE_FLAG = {
        {IPC,     CLONE_NEWIPC},
        {NETWORK, CLONE_NEWNET},
        {MOUNT,   CLONE_NEWNS},
        {PID,     CLONE_NEWPID},
        // {TIME, CLONE_NEWTIME}, // It seems that it is not supported on my Linux version
        {USER, CLONE_NEWUSER},
        {UTS, CLONE_NEWUTS},
};

const std::unordered_map<ns_type, std::string> NS_SHORTCUT = {
        {IPC, "ipc"},
        {NETWORK, "net"},
        {MOUNT, "mnt"},
        {PID, "pid"},
        {USER, "user"},
        {TIME, "time"},
        {UTS, "uts"}
};

void deleteCharPtrArray(char **av) {
    if (av) {
        for (size_t i = 0; av[i]; ++i) {
            delete[] av[i];
        }
        delete[] av;
    }
}

std::unique_ptr<char *[], void(*)(char **)> createCharPtrArray(const std::vector<std::string> &tokens) {
    std::unique_ptr<char *[], void(*)(char **)> ptr(new char *[tokens.size() + 1], deleteCharPtrArray);
    ptr[tokens.size()] = nullptr;
    for (size_t i = 0; i < tokens.size(); ++i) {
        ptr[i] = new char[tokens[i].length() + 1];
        std::strcpy(ptr[i], tokens[i].c_str());
    }
    return ptr;
}

std::string get_ns_handle_path(ns_type tp, pid_t pid){
    std::stringstream path;
    path << "/proc/" << pid << "/ns/" << NS_SHORTCUT.at(tp);
    return path.str();
}

int get_ns_handle(ns_type tp, pid_t pid){
    auto path = get_ns_handle_path(tp, pid);
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1){
        throw std::runtime_error("Failed to open namespace handle!");
    }
    return fd;
}

static std::shared_ptr<ns> create_namespace_entry(ns_type tp, const std::string &ns_name, int fd, pid_t pid){
    std::shared_ptr<ns> new_ns;
    switch (tp) {
        case IPC:
            new_ns = std::make_shared<ipc_ns>(ns_name, fd, tp, pid);
            break;
        case NETWORK:
            new_ns = std::make_shared<net_ns>(ns_name, fd, tp, pid);
            break;
        case MOUNT:
            new_ns = std::make_shared<mnt_ns>(ns_name, fd, tp, pid);
            break;
        case PID:
            new_ns = std::make_shared<pid_ns>(ns_name, fd, tp, pid);
            break;
        case TIME:
            new_ns = std::make_shared<time_ns>(ns_name, fd, tp, pid);
            break;
        case USER:
            new_ns = std::make_shared<user_ns>(ns_name, fd, tp, pid);
            break;
        case UTS:
            new_ns = std::make_shared<uts_ns>(ns_name, fd, tp, pid);
            break;
        default:
            throw std::runtime_error("Unsupported namespace type!");
    }
    return new_ns;
}

#endif //JOCKER_SERVER_COMMON_H
