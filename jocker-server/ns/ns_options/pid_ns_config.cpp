#include "pid_ns_config.h"

pid_ns_config::pid_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}
