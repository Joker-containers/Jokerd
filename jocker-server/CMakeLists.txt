cmake_minimum_required(VERSION 3.22)
project(jocker_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(jocker_server main.cpp
        container/container.cpp
        container/container_cluster.h
        container/container_cluster.cpp
        cgroup/cgroup.cpp
        ns/ns_group.cpp)

find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)

target_include_directories(${PROJECT_NAME} PUBLIC ./container)
target_include_directories(${PROJECT_NAME} PUBLIC ./cgroup)
target_include_directories(${PROJECT_NAME} PUBLIC ./ns)