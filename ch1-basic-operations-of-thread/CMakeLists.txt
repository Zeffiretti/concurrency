cmake_minimum_required(VERSION 3.0)
project(ch1)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}_node main.cc)
target_link_libraries(${PROJECT_NAME}_node ${CMAKE_THREAD_LIBS_INIT})
