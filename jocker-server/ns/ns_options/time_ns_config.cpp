#include "time_ns_config.h"

time_ns_config::time_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}
