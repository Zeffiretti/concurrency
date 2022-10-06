# 条件变量
[toc]

## 条件变量的概念
条件变量（Condition Variable）是一种同步机制，它允许一个线程等待某个条件的发生。条件变量与互斥量一起使用，以避免出现**虚假唤醒**（spurious wakeup）的情况。

## `condition_variable` 工作机制
  1. 一个线程等待某个条件的发生，它首先要获得互斥量的所有权，然后调用 `wait()` 方法，将自己阻塞，直到条件发生。
  2. 另一个线程改变了条件的状态，它首先要获得互斥量的所有权，然后调用 `notify_one()` 方法，唤醒一个等待该条件的线程。
  3. 唤醒的线程获得互斥量的所有权，然后检查条件是否已经发生，如果条件已经发生，它就可以继续执行，否则，它将继续等待。

## 条件变量使用
### `condition_variable` 类
`condition_variable` 类提供了条件变量的功能，它的成员函数如下：
  - `void wait(unique_lock<mutex>& lock)`：将当前线程阻塞，直到条件变量被唤醒。
  - `template <class Predicate> void wait(unique_lock<mutex>& lock, Predicate pred)`：将当前线程阻塞，直到条件变量被唤醒或者 `pred()` 返回 `true`。
  - `void notify_one()`：唤醒一个等待该条件变量的线程。
  - `void notify_all()`：唤醒所有等待该条件变量的线程。

### `condition_variable` 示例
下面的示例演示了如何使用条件变量实现一个生产者-消费者模型：
```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
queue<int> q;

void producer(int id) {
    for (int i = 0; i < 10; ++i) {
        unique_lock<mutex> lock(mtx);
        q.push(i);
        cout << "producer " << id << " produced " << i << endl;
        cv.notify_one();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void consumer(int id) {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return !q.empty(); });
        int i = q.front();
        q.pop();
        cout << "consumer " << id << " consumed " << i << endl;
        if (i == 9) break;
    }
}

int main() {
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);
    p1.join();
    p2.join();
    c1.join();
    c2.join();
    return 0;
}
```

## `condition_variable_any` 类
`condition_variable_any` 类提供了条件变量的功能，它的成员函数与 `condition_variable` 类的成员函数基本相同，不同之处在于，`condition_variable_any` 类的成员函数的参数类型为 `unique_lock<mutex>`，而 `condition_variable` 类的成员函数的参数类型为 `unique_lock<mutex>` 或 `unique_lock<recursive_mutex>`。

## `condition_variable_any` 示例
下面的示例演示了如何使用 `condition_variable_any` 类实现一个生产者-消费者模型：
```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable_any cv;
queue<int> q;

void producer(int id) {
    for (int i = 0; i < 10; ++i) {
        unique_lock<mutex> lock(mtx);
        q.push(i);
        cout << "producer " << id << " produced " << i << endl;
        cv.notify_one();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void consumer(int id) {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return !q.empty(); });
        int i = q.front();
        q.pop();
        cout << "consumer " << id << " consumed " << i << endl;
        if (i == 9) break;
    }
}

int main() {
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);
    p1.join();
    p2.join();
    c1.join();
    c2.join();
    return 0;
}
```
