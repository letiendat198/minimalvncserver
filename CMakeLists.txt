cmake_minimum_required(VERSION 3.29)
project(minimalvncserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(minimalvncserver src/main.cpp
        lib/win32screencap.cpp
        lib/util.cpp
        include/util.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(minimalvncserver Threads::Threads)

include_directories("include")
include_directories("D:\\Projects\\CLib\\libvncserver\\build\\include")
include_directories("D:\\Projects\\CLib\\libvncserver\\include")
target_link_libraries(minimalvncserver "D:\\Projects\\CLib\\libvncserver\\build\\libvncserver.a")

if(WIN32)
    target_link_libraries(minimalvncserver wsock32 ws2_32)
endif()


