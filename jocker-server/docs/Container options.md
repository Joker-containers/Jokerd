# Container options

This class is the collection of parameters required to create a container.

### Fields

* `ns_options` namespaces - `ns_options` instance, contains info about desired namespaces for this container.
* `cgroup_options` _cgroup_options - `cgroup_options` instance, contains info about desired cgroups for this container.
* `std::string` bin_path - a path to the binary to be launched inside the container
* `std::vector<std::string>` bin_arguments - arguments for launching the binary
* `std::string` container_name - container name 