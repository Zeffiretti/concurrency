#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ch6-singleton-pattern/singleton.h"

#define UNUSED(x) (void)(x)

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);
  std::cout << "main thread starts." << std::endl;
  std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

  // std::thread t1([]() {
  //   std::cout << "t1 thread starts." << std::endl;
  //   std::cout << "t1 thread id: " << std::this_thread::get_id() << std::endl;
  // });
  // t1.join();

  Singleton& singleton = Singleton::getInstance();

  std::cout << "main thread ends." << std::endl;

  return 0;
}
