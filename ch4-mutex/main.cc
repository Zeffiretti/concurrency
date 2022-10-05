#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ch4-mutex/dead_lock.h"
#include "ch4-mutex/net_server.h"

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

  NetServer game_server;

  // demo of dead lock
  // DeadLockThread dead_lock_thread;

  NoDeadLockThread no_dead_lock_thread;

  std::cout << "main thread ends." << std::endl;

  return 0;
}
