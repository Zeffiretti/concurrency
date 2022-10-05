#ifndef CH3_MULTI_THREADS_NET_SERVER
#define CH3_MULTI_THREADS_NET_SERVER
#include <iostream>
#include <list>
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
  }
  void receiveCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      std::cout << "receiveCmd: " << i << std::endl;
      msg_recv_queue.push_back(i);
    }
  }
  void processCmd(int cmd) {
    for (int i = 0; i < cmd; ++i) {
      if (!msg_recv_queue.empty()) {
        std::cout << "processCmd: " << msg_recv_queue.front() << std::endl;
        msg_recv_queue.pop_front();
      }
    }
  }

 private:
  std::list<int> msg_recv_queue;
};

#endif /* CH3_MULTI_THREADS_NET_SERVER */
