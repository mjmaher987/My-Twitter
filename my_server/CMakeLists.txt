cmake_minimum_required(VERSION 3.17)
project(my_server C)

set(CMAKE_C_STANDARD 99)

add_executable(my_server main.c)

target_link_libraries(my_server ws2_32)