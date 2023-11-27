#include "container.h"


container::container(const container_options &opts) {
    m_cname = opts.container_name;
    // mount -t proc proc /proc In the new fs

}