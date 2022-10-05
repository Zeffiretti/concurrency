#ifndef CH3_MULTI_THREADS_THREAD_DATA
#define CH3_MULTI_THREADS_THREAD_DATA
#include <iostream>
#include <thread>
#include <vector>

class MThread {
 public:
  MThread() {
    threads.clear();
    std::cout << "MThread default constructor." << std::endl;
  }
  MThread(const std::vector<int>& vals) : values(vals) {
    threads.clear();
    std::cout << "MThread constructor." << std::endl;
  }
  void run() {
    for (int i = 0; i < 10; ++i) {
      threads.emplace_back(std::thread(&MThread::printThread, this));
    }
    for (auto& thread : threads) {
      thread.join();
    }
  }
  void printThread() {
    std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << "values are ";
    for (int& i : values) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "thread ends." << std::endl;
  }

 private:
  std::vector<int> values = {1, 2, 3};  // The shared data
  std::vector<std::thread> threads;
};

#endif /* CH3_MULTI_THREADS_THREAD_DATA */
