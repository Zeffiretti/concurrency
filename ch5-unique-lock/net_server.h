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
    std::thread t1(&NetServer::receiveCmd, this, 100000);
    std::thread t2(&NetServer::processCmd, this, 100000);

    t1.join();
    t2.join();
  }
  void receiveCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      std::cout << "receiveCmd: " << i << std::endl;
      std::unique_lock<std::mutex> lock(msg_recv_queue_mutex, std::try_to_lock);
      // std::unique_lock<std::mutex> lock(msg_recv_queue_mutex, std::defer_lock);
      // lock.lock();
      if (lock.owns_lock()) {
        if (!msg_tmp.empty()) {
          msg_recv_queue.splice(msg_recv_queue.end(), msg_tmp);
        }
        msg_recv_queue.push_back(i);
      } else {
        msg_tmp.push_back(i);
        std::cout << "receiveCmd: lock failed" << std::endl;
      }
    }
  }
  bool popCmd(int& cmd) {
    std::unique_lock<std::mutex> lock(msg_recv_queue_mutex);
    if (!msg_recv_queue.empty()) {
      cmd = msg_recv_queue.front();
      msg_recv_queue.pop_front();
      return true;
    }
    return false;
  }
  void processCmd(int cmd) {
    int rcmd;
    for (int i = 0; i < cmd; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      bool res = popCmd(rcmd);
      if (res) {
        std::cout << "processCmd: " << rcmd << std::endl;
      } else {
        std::cout << "processCmd: no cmd" << std::endl;
      }
    }
  }

 private:
  std::list<int> msg_recv_queue;
  std::list<int> msg_tmp;
  std::mutex msg_recv_queue_mutex;
};

#endif /* CH5_MULTI_THREADS_NET_SERVER */
