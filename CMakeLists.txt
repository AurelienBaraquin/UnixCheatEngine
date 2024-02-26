cmake_minimum_required(VERSION 3.11)
project(UnixCheatEngine)

find_package(raylib REQUIRED)

add_subdirectory(raylib-cpp)

include_directories(include/Memory)
include_directories(include/Graphical)
include_directories(include)

file(GLOB MEMORY_SOURCES "src/Memory/*.cpp")
file(GLOB GRAPHICAL_SOURCES "src/Graphical/*.cpp")
file(GLOB SOURCES "src/*.cpp")


add_executable(${PROJECT_NAME} ${SOURCES} ${MEMORY_SOURCES} ${GRAPHICAL_SOURCES})

target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PRIVATE raylib-cpp/include)
