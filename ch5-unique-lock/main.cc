#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ch5-unique-lock/net_server.h"

#define UNUSED(x) (void)(x)

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);
  std::cout << "main thread starts." << std::endl;
  std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

  NetServer server;

  std::cout << "main thread ends." << std::endl;

  return 0;
}
