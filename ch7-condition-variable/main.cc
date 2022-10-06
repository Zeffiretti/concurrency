#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ch7-condition-variable/parallel_list.h"

#define UNUSED(x) (void)(x)

void pushValueThread(ParallelList<int>& list, int nums) {
  for (int i = 0; i < nums; ++i) {
    // std::this_thread::sleep_for(std::chrono::microseconds(1));
    if (list.push_back(i)) {
      std::cout << "pushed " << i << std::endl;
    } else {
      std::cout << "push " << i << " failed" << std::endl;
    }
  }
}

void popValueThread(ParallelList<int>& list) {
  int value;
  while (list.pop_front(value)) {
    std::this_thread::sleep_for(std::chrono::microseconds(1));
    std::cout << "pop value: " << value << std::endl;
  }
}

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);
  std::cout << "main thread starts." << std::endl;
  std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

  ParallelList<int> list;
  int num_producer = 2;
  int num_consumer = 3;

  std::vector<std::thread> producers(num_producer);
  for (int i = 0; i < num_producer; ++i) {
    producers[i] = std::thread(pushValueThread, std::ref(list), 1000);
  }

  std::vector<std::thread> consumers(num_consumer);
  for (int i = 0; i < num_consumer; ++i) {
    consumers[i] = std::thread(popValueThread, std::ref(list));
  }

  for (int i = 0; i < num_producer; ++i) {
    producers[i].join();
  }

  for (int i = 0; i < num_consumer; ++i) {
    consumers[i].join();
  }

  std::cout << "main thread ends." << std::endl;

  return 0;
}
