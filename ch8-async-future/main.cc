#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#define UNUSED(x) (void)(x)

int futureThread() {
  std::cout << "future thread starts with thread id: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));  // sllep for 500 ms
  std::cout << "future thread ends." << std::endl;
  return 5;
}

float packageThread(int id) {
  std::cout << "package thread starts with thread id: " << std::this_thread::get_id() << " and param id: " << id
            << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));  // sllep for 500 ms
  std::cout << "package thread ends." << std::endl;
  return 15.0f;
}

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);
  std::cout << "main thread starts." << std::endl;
  std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

  std::future<int> result = std::async(std::launch::deferred, futureThread);
  std::cout << "main thread continues." << std::endl;
  // result.wait();  // wait here, until the future thread ends
  std::cout << "result: " << result.get() << std::endl;  // The main thread will wait for the result

  std::packaged_task<float(int)> task(packageThread);
  std::future<float> result2 = task.get_future();
  std::thread t(std::move(task), 10);
  std::cout << "main thread continues." << std::endl;
  std::cout << "result2: " << result2.get() << std::endl;
  t.join();

  std::cout << "main thread ends." << std::endl;

  return 0;
}
