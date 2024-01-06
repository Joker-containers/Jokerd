#ifndef JOCKER_SERVER_SYSCALL_WRAPPER_H
#define JOCKER_SERVER_SYSCALL_WRAPPER_H

#include <cstring>
#include <string>
#include <sstream>

constexpr int SYSCALL_FAIL = -1;

template<typename Func, typename... Args>
int syscall_wrapper(Func syscall, const std::string &syscall_name, Args... args) {
    int result = 0;
    while (true){
        result = syscall(args...);
        if (result == SYSCALL_FAIL) {
            if (errno == EINTR) continue;
            else{
                std::stringstream error_message;
                error_message << "Error in syscall " << syscall_name << ": " << strerror(errno) << std::endl;
                throw std::runtime_error(error_message.str());
            }
        }
        break;
    }
    return result;
}

#endif //JOCKER_SERVER_SYSCALL_WRAPPER_H
