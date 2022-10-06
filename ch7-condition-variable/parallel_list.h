#ifndef CH7_CONDITION_VARIABLE_PARALLEL_LIST
#define CH7_CONDITION_VARIABLE_PARALLEL_LIST
#include <condition_variable>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <utility>

template <typename T>
class ParallelList {
 public:
  ParallelList() = default;

  bool push_back(const T& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    list_.push_back(value);
    cond_.notify_all();
    return true;
  }

  bool pop_front(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait_for(lock, std::chrono::milliseconds(1000), [this]() { return !list_.empty(); });
    if (!list_.empty()) {
      value = list_.front();
      list_.pop_front();
      return true;
    }
    return false;
  }

 private:
  std::list<T> list_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

#endif /* CH7_CONDITION_VARIABLE_PARALLEL_LIST */
