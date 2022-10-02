cmake_minimum_required(VERSION 3.0)
project(concurrency)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(CONCURRENCY_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CONCURRENCY_ROOT_DIR})

add_executable(${PROJECT_NAME}_node main.cc)
target_link_libraries(${PROJECT_NAME}_node ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(ch1-basic-operations-of-thread)
