cmake_minimum_required(VERSION 3.0)
project(data_structure)
find_package(Threads REQUIRED)

add_library(data_structure_lib
  data_structure.cc
  thread_safe_stack.cc
  thread_safe_stack.h
)
target_include_directories(data_structure_lib PUBLIC
  ${CONCURRENCY_ROOT_DIR}
)
target_link_libraries(data_structure_lib ${CMAKE_THREAD_LIBS_INIT})
