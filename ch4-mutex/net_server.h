#ifndef CH4_MULTI_THREADS_NET_SERVER
#define CH4_MULTI_THREADS_NET_SERVER
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
    std::thread t2(&NetServer::processCmd, this, 100000);

    t1.join();
    t2.join();
  }
  void receiveCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      std::cout << "receiveCmd: " << i << std::endl;
      // msg_recv_queue_mutex.lock();
      std::lock_guard<std::mutex> lock(msg_recv_queue_mutex);
      msg_recv_queue.push_back(i);
      // msg_recv_queue_mutex.unlock();
    }
  }
  bool popCmd(int& cmd) {
    std::lock_guard<std::mutex> lock(msg_recv_queue_mutex);
    // msg_recv_queue_mutex.lock();
    if (!msg_recv_queue.empty()) {
      cmd = msg_recv_queue.front();
      msg_recv_queue.pop_front();
      // msg_recv_queue_mutex.unlock();
      return true;
    }
    // msg_recv_queue_mutex.unlock();
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
  std::mutex msg_recv_queue_mutex;
};

#endif /* CH4_MULTI_THREADS_NET_SERVER */
