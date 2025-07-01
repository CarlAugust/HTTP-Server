cmake_minimum_required(VERSION 3.10)
project(http)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable debug info by default
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

file(GLOB SOURCES "src/*.c")

# Add the source files
add_executable(http ${SOURCES})

target_include_directories(http PRIVATE src/ include/)

target_link_libraries(http m pthread raylib)