#ifndef DATA_STRUCTURE_THREAD_SAFE_STACK
#define DATA_STRUCTURE_THREAD_SAFE_STACK
#include <cstddef>
#include <deque>
#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>

namespace data_structure {

struct empty_stack : std::exception {
  const char* what() const throw() { return "empty stack"; }
};

template <typename T>
class ThreadSafeStack {
 public:
  ThreadSafeStack();
  ThreadSafeStack(const ThreadSafeStack&);
  ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;  // disable copy assignment

  void push(T new_value);
  std::shared_ptr<T> pop();
  void pop(T& value);
  bool empty() const;

 private:
  std::stack<T> data;
  mutable std::mutex m;
};

}  // namespace data_structure

#endif /* DATA_STRUCTURE_THREAD_SAFE_STACK */
