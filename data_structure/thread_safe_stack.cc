#include "data_structure/thread_safe_stack.h"

#include <future>
#include <iostream>
#include <memory>
#include <mutex>

namespace data_structure {

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack() : data(std::stack<T>()) {}

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack(const ThreadSafeStack& other) {
  std::lock_guard<std::mutex> lock(other.m);
  data = other.data;  // copy the data in constructor
}

template <typename T>
void ThreadSafeStack<T>::push(T new_value) {
  std::lock_guard<std::mutex> lock(m);
  data.push(new_value);
}

template <typename T>
std::shared_ptr<T> ThreadSafeStack<T>::pop() {
  std::unique_lock<std::mutex> lock(m);
  if (data.empty()) {
    throw empty_stack();
  }
  std::shared_ptr<T> const res(
      std::make_shared<T>(data.top()));  // Assign the top value to a shared_ptr before the stack is popped
  data.pop();
  return res;
}

template <typename T>
void ThreadSafeStack<T>::pop(T& value) {
  std::unique_lock<std::mutex> lock(m);
  if (data.empty()) {
    throw empty_stack();
  }
  value = data.top();
  data.pop();
}

template <typename T>
bool ThreadSafeStack<T>::empty() const {
  std::lock_guard<std::mutex> lock(m);
  return data.empty();
}

}  // namespace data_structure