cmake_minimum_required(VERSION 3.11)
project(UnixCheatEngine)

add_subdirectory(raylib-cpp)

include_directories(include)
include_directories(include/Memory)

file(GLOB SOURCES "src/*.cpp")
file(GLOB MEMORY_SOURCES "src/Memory/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${MEMORY_SOURCES})

target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PRIVATE raylib-cpp/include)

find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} raylib)
