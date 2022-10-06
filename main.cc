#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#include "background_task.h"
#include "parallel_accumulate.h"
#include "thread_management.h"

#define UNUSED(x) (void)(x)

void hello() {
  std::cout << std::this_thread::get_id();
  std::cout << "Hello Concurrent World\n";
}

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  // Randomly generate a vector, containing 1000000 items in range [0 100]
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 100);
  std::vector<int> v(10000000);
  std::generate(v.begin(), v.end(), [&]() { return dis(gen); });

  // Time the parallel accumulate function
  auto start = std::chrono::high_resolution_clock::now();
  int sum = parallel_accumulate(v.begin(), v.end(), 0);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Sum: " << sum << std::endl;
  std::cout << "Time: " << diff.count() << "s" << std::endl;

  // Time the std::accumulate function
  start = std::chrono::high_resolution_clock::now();
  sum = std::accumulate(v.begin(), v.end(), 0);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  std::cout << "Sum: " << sum << std::endl;
  std::cout << "Time: " << diff.count() << "s" << std::endl;

  return 0;
}
