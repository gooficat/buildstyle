cmake_minimum_required(VERSION 3.10)
project(BuildStyle)

find_package(SDL2 REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)