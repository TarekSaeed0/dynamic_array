cmake_minimum_required(VERSION 3.24.2)
project(dynamic_array)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${CMAKE_PROJECT_NAME} main.c)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c89 -Werror -Wall -Wextra -pedantic)

