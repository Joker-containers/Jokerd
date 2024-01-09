#ifndef JOCKER_SERVER_USER_NS_H
#define JOCKER_SERVER_USER_NS_H


#include "ns.h"

class user_ns: public ns {
public:
    user_ns() = delete;

    explicit user_ns(std::string name, std::vector<std::string> uid_map,
                     std::vector<std::string> gid_map);

    user_ns(std::string name, int fd, pid_t process_pid);

    void external_setup_ns() override;

    void internal_setup_ns() override;

    void init_internal() override;

    void init_external() override;

    void configure_ns() override;

    [[nodiscard]] int get_type() const override{
        return USER;
    }

private:
    std::vector<std::string> uid_mappings;
    std::vector<std::string> gid_mappings;
};


#endif //JOCKER_SERVER_USER_NS_H
