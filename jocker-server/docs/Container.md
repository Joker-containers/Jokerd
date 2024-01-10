# Container

Container is the entity, which provides means of the virtualization utilizing Cgroups and Namespaces.

## General description

Namespaces is the kernel feature providing a way to virtualize the different aspects of program-system interaction.
There are numerous namespaces, each responsible for some part of that interaction.
The more namespaces are used, the better isolation from the host system can be ensured.

Cgroups feature is somewhat similar, but in terms of hardware resources usage restriction.

You can read more about them here: <br>
https://man7.org/linux/man-pages/man7/namespaces.7.html <br>
https://man7.org/linux/man-pages/man7/cgroups.7.html <br>

The container function is to put a program inside the specified namespaces and cgroups.
This requires either the creation of new or joining the existing ones, and the daemon managing all of them should be aware of that actions as well.


## Usage (API)



## Implementation details

### Fields

* `std::string` <b>_cname</b> - name of the container (for managing purposes) <br>
* `cgroup` <b>_cgroup</b> - `cgroup` instance, responsible for Cgroups management
* `ns_group` <b>_namespaces</b> - `ns_group` instance, responsible for Namespaces management

### Container construction and setup

On construction, container has to receive a few parameters: <br>
`container(const container_options &opts, d_resources &daemon_resources);`

First argument contains options for a container, the second one contains a resources required to use this options with.
These options include binary name, arguments to launch with and container name.
Moreover, there are namespace and cgroup options. 
Namespace options contain names of the namespaces to join or to create.
The already existing ones are registered in daemon resources, and they can be entered in using info contained in `daemon_resources` argument.


### Namespaces usage



### Cgroups usage





