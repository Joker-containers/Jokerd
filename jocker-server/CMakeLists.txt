cmake_minimum_required(VERSION 3.22)
project(jocker_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(jocker_server main.cpp
        container/container.cpp
        container/container_cluster.h
        container/container_cluster.cpp
        cgroup/cgroup.cpp
        ns/ns_group.cpp
        ns/ns_types/ns.h
        ns/ns_types/ns.cpp
        ns/ns_options.cpp
        container/container_options.cpp
        cgroup/cgroup_options.cpp
        ns/ns_types/ipc_ns.cpp
        ns/ns_types/net_ns.cpp
        ns/ns_types/mnt_ns.cpp
        ns/ns_types/pid_ns.cpp
        ns/ns_types/time_ns.cpp
        ns/ns_types/user_ns.cpp
        ns/ns_types/uts_ns.cpp
        ns/ns_pool.cpp)

find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)

target_include_directories(${PROJECT_NAME} PUBLIC ./container)
target_include_directories(${PROJECT_NAME} PUBLIC ./cgroup)
target_include_directories(${PROJECT_NAME} PUBLIC ./ns)