#include "ipc_ns_config.h"

ipc_ns_config::ipc_ns_config(std::ifstream &file, int id) : ns_config() {
    set_id(id);
}
