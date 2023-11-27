#ifndef JOCKER_SERVER_NS_H
#define JOCKER_SERVER_NS_H

#include <string>

typedef int fd_t;

enum ns_type{
    IPC,
    NETWORK,
    MOUNT,
    PID,
    TIME,
    USER,
    UTS
};

class ns {
public:
    [[nodiscard]] const std::string &get_name(){
        return m_name;
    }
    [[nodiscard]] fd_t get_fd() const{
        return m_fd;
    }
private:
    std::string m_name;
    fd_t m_fd;
};


#endif //JOCKER_SERVER_NS_H
