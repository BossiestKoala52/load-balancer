cmake_minimum_required(VERSION 3.26)
project(untitled3 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled3 User.c
        InstanceHost.c
        LoadBalancer.c)
