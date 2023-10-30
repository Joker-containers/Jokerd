#include <iostream>

void create_container(){
    // Creates container.
    // First, a cgroup directory should be created and set up.
    // Then, call clone(). Clone unshares his namespaces, while parent sets them up.
    // Finally, after some kind of message about setup finish, child does exec()
    // From that moment it is the requested program launched in the container
    // Probably this method will delegate all the work to the container's constructor
}

void list_containers(){
    // Lists containers and relevant info, probably by some internal method of container cluster.
}

void get_container_info(){
    // Provides a detailed info about the specified container
}

void remove_container(){
    // Kills processes inside the container and releases all the resources. Clears related directories etc.
}

int main() {
    // First thing main does: checks whether cgroup "Jocker" directory exists, creates if not.
    // Then, waiting commands from cli (using sockets?)
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
