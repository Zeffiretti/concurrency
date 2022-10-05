#ifndef CH5_MULTI_THREADS_NET_SERVER
#define CH5_MULTI_THREADS_NET_SERVER
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

// Demo of multi-threading with network server
// one thread: collect player commands (std::list)
// one thread: process player commands (std::list)
class NetServer {
 public:
  NetServer() {
    msg_recv_queue.clear();
    std::thread t1(&NetServer::receiveCmd, this, 1000);
    std::thread t2(&NetServer::processCmd, this);

    t1.join();
    t2.join();
  }
  void receiveCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      std::unique_lock<std::mutex> lock(msg_recv_queue_mutex, std::try_to_lock);
      // std::mutex* ptx = lock.release(); // Later, we should use ptx to lock manually by ourselves
      // std::unique_lock<std::mutex> lock(msg_recv_queue_mutex,
      //                                   std::defer_lock);  // msg_recv_queue_mutex has not been locked here
      //                                                      // lock.lock();
      if (lock.owns_lock()) {
        // if (lock.try_lock()) {
        std::cout << "receiveCmd: ";
        for (auto it = msg_tmp.begin(); it != msg_tmp.end(); ++it) {
          std::cout << " " << *it;
        }
        if (!msg_tmp.empty()) {
          msg_recv_queue.splice(msg_recv_queue.end(), msg_tmp);
        }
        std::cout << i << std::endl;
        msg_recv_queue.push_back(i);
      } else {
        msg_tmp.push_back(i);
        std::cout << "receiveCmd: lock failed.   ";
        std::cout << "msg_tmp.size() = " << msg_tmp.size() << ".";
        for (auto it = msg_tmp.begin(); it != msg_tmp.end(); ++it) {
          std::cout << " " << *it;
        }
        std::cout << std::endl;
      }
    }
    while (!msg_tmp.empty()) {
      std::unique_lock<std::mutex> lock(msg_recv_queue_mutex, std::try_to_lock);
      // std::mutex* ptx = lock.release();  // Later, we should use ptx to lock manually by ourselves
      // std::unique_lock<std::mutex> lock(msg_recv_queue_mutex,
      //                                   std::defer_lock);  // msg_recv_queue_mutex has not been locked here
      //                                                      // lock.lock();
      if (lock.owns_lock()) {
        // if (lock.try_lock()) {
        std::cout << "receiveCmd: ";
        for (auto it = msg_tmp.begin(); it != msg_tmp.end(); ++it) {
          std::cout << " " << *it;
        }
        msg_recv_queue.splice(msg_recv_queue.end(), msg_tmp);
      }
    }
  }
  bool popCmd(int& cmd) {
    std::unique_lock<std::mutex> lock(msg_recv_queue_mutex);
    // std::this_thread::sleep_for(std::chrono::microseconds(500));
    if (!msg_recv_queue.empty()) {
      cmd = msg_recv_queue.front();
      msg_recv_queue.pop_front();
      return true;
    }
    return false;
  }
  void processCmd() {
    int rcmd;
    bool status = true;
    while (status) {
      // 50 us for one try
      // std::this_thread::sleep_for(std::chrono::microseconds(50));
      auto start = std::chrono::system_clock::now();
      status = false;
      auto end = std::chrono::system_clock::now();
      // if the consumed time is less than 5s, continue trying
      while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 5) {
        status = popCmd(rcmd);
        if (status) {
          std::cout << "processCmd: " << rcmd << std::endl;
          break;
        }
        end = std::chrono::system_clock::now();
      }
    }
    std::cout << "We have waited for at least 5 seconds, but no command is received. EXITING.." << std::endl;
  }

 private:
  std::list<int> msg_recv_queue;
  std::list<int> msg_tmp;
  std::mutex msg_recv_queue_mutex;
};

#endif /* CH5_MULTI_THREADS_NET_SERVER */
