#ifndef CH4_MUTEX_DEAD_LOCK
#define CH4_MUTEX_DEAD_LOCK
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

class DeadLockThread {
 public:
  DeadLockThread() {
    msg_recv_queue.clear();
    std::thread t1(&DeadLockThread::receiveCmd, this, 1000);
    std::thread t2(&DeadLockThread::processCmd, this, 100000);

    t1.join();
    t2.join();
  }
  void receiveCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      std::cout << "receiveCmd: " << i << std::endl;
      // dead-lock-prone method
      msg_recv_queue_mutex.lock();
      msg_send_queue_mutex.lock();
      msg_recv_queue.push_back(i);
      msg_send_queue_mutex.unlock();
      msg_recv_queue_mutex.unlock();
    }
  }
  bool popCmd(int& cmd) {
    // dead-lock-prone method
    msg_send_queue_mutex.lock();
    msg_recv_queue_mutex.lock();
    if (!msg_recv_queue.empty()) {
      cmd = msg_recv_queue.front();
      msg_recv_queue.pop_front();
      msg_recv_queue_mutex.unlock();
      msg_send_queue_mutex.unlock();
      return true;
    }
    msg_recv_queue_mutex.unlock();
    msg_send_queue_mutex.unlock();
    return false;
  }
  void processCmd(int cmd) {
    int rcmd;
    for (int i = 0; i < cmd; ++i) {
      bool res = popCmd(rcmd);
      if (res) {
        std::cout << "processCmd: " << rcmd << std::endl;
      }
    }
  }

 private:
  std::list<int> msg_recv_queue;
  std::list<int> msg_send_queue;
  std::mutex msg_recv_queue_mutex;
  std::mutex msg_send_queue_mutex;
};

class NoDeadLockThread {
 public:
  NoDeadLockThread() {
    msg_recv_queue.clear();
    std::thread t1(&NoDeadLockThread::receiveCmd, this, 1000);
    std::thread t2(&NoDeadLockThread::processCmd, this, 100000);

    t1.join();
    t2.join();
  }
  void receiveCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      std::cout << "receiveCmd: " << i << std::endl;
      std::lock(msg_recv_queue_mutex, msg_send_queue_mutex);
      std::lock_guard<std::mutex> lock1(msg_recv_queue_mutex, std::adopt_lock);
      std::lock_guard<std::mutex> lock2(msg_send_queue_mutex, std::adopt_lock);
      msg_recv_queue.push_back(i);
      // msg_recv_queue_mutex.unlock();
      // msg_send_queue_mutex.unlock();
    }
  }
  bool popCmd(int& cmd) {
    std::lock(msg_recv_queue_mutex, msg_send_queue_mutex);
    std::lock_guard<std::mutex> lock1(msg_recv_queue_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(msg_send_queue_mutex, std::adopt_lock);
    if (!msg_recv_queue.empty()) {
      cmd = msg_recv_queue.front();
      msg_recv_queue.pop_front();
      // msg_recv_queue_mutex.unlock();
      // msg_send_queue_mutex.unlock();
      return true;
    }
    // msg_recv_queue_mutex.unlock();
    // msg_send_queue_mutex.unlock();
    return false;
  }
  void processCmd(int cmd) {
    int rcmd;
    for (int i = 0; i < cmd; ++i) {
      bool res = popCmd(rcmd);
      if (res) {
        std::cout << "processCmd: " << rcmd << std::endl;
      }
    }
  }

 private:
  std::list<int> msg_recv_queue;
  std::list<int> msg_send_queue;
  std::mutex msg_recv_queue_mutex;
  std::mutex msg_send_queue_mutex;
};

#endif /* CH4_MUTEX_DEAD_LOCK */
