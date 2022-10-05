cmake_minimum_required(VERSION 3.0)
project(concurrency)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
# support GDB debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CONCURRENCY_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CONCURRENCY_ROOT_DIR})

add_executable(${PROJECT_NAME}_node main.cc)
target_link_libraries(${PROJECT_NAME}_node ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(ch1-basic-operations-of-thread)
add_subdirectory(ch2-pass-arguments-to-thread)
add_subdirectory(ch3-multi-threads)
add_subdirectory(ch4-mutex)
add_subdirectory(ch5-unique-lock)