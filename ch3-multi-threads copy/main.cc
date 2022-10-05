#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ch3-multi-threads/net_server.h"
#include "ch3-multi-threads/thread_data.h"

#define UNUSED(x) (void)(x)

void myPrint(int seconds) {
  std::cout << "[" << seconds << "]"
            << "thread id: " << std::this_thread::get_id() << " starts." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
  std::cout << "[" << seconds << "]"
            << "thread id: " << std::this_thread::get_id() << " ends." << std::endl;
}

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);
  std::cout << "main thread starts." << std::endl;
  std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back(myPrint, 11 - i);  // implicit move
  }

  for (auto& thread : threads) {
    thread.join();
  }

  MThread mthread;
  mthread.run();

  MThread mthread2({4, 5, 6});
  mthread2.run();

  NetServer game_server;

  std::cout << "main thread ends." << std::endl;

  return 0;
}
