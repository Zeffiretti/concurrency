cmake_minimum_required(VERSION 3.0)
project(ch4)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}_node main.cc)
target_include_directories(${PROJECT_NAME}_node PUBLIC
  ${CONCURRENCY_ROOT_DIR}
)
target_link_libraries(${PROJECT_NAME}_node ${CMAKE_THREAD_LIBS_INIT})
