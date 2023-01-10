cmake_minimum_required(VERSION 3.17)
project(client C)

set(CMAKE_C_STANDARD 99)

add_executable(client main.c slre.c)
target_link_libraries(client ws2_32)