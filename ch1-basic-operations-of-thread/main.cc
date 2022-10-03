#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#define UNUSED(x) (void)(x)

class MyThread {
 public:
  MyThread() {
    std::cout << "construct function" << std::endl;
    std::cout << "this pointer: " << this << std::endl;
  }
  MyThread(const MyThread& other) {
    std::cout << "copy construct function" << std::endl;
    // print this pointer
    this->ref_ = other.ref_;
    std::cout << "this pointer: " << this << std::endl;
  }
  MyThread(MyThread&& other) noexcept {
    std::cout << "move construct function" << std::endl;
    // print this pointer
    this->ref_ = other.ref_;
    std::cout << "this pointer: " << this << std::endl;
  }
  // MyThread(int& r) : ref_(r) { std::cout << "ref function" << std::endl; }  // one bug-prone method
  ~MyThread() { std::cout << "destruct function" << std::endl; }
  void operator()() {  //不能带参数
    std::cout << "my thread id: " << std::this_thread::get_id() << std::endl;
    for (int i = 0; i < 1; ++i) {
      std::cout << ref_ << std::endl;
    }
    std::cout << "my thread end" << std::endl;
  }

 private:
  //   int& ref_;  // one bug-prone member
  int ref_ = 10;
};

void myPrint() {
  std::cout << "my thread id: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "my thread end" << std::endl;
}

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  // 1. The main thread is automatically created and started when the program starts.
  std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

  // 2. Since main() is the program entry,
  // the new thread should be created manually by user after the main thread is created,
  // The program ends when the main thread ends.
  // Meanwhile, if the new thread is still running, the program should wait for the new thread to end.
  // Hence, in main thread, we should use join() to wait for the new thread to end.

  std::thread t(myPrint);  // Create a new thread, which executes myPrint()
  if (t.joinable()) {
    t.join();  // If comment this line, the program pop up an error : terminate called without an active exception
  }
  // t.detach();

  int a = 10;
  MyThread my_thread;
  std::cout << "object constructed." << std::endl;
  std::thread t2(my_thread);  // Create a new thread, which executes my_thread()
  // One problem is that after calling t2.detach(), the main thread will not wait for the new thread to end.
  // Hence, the varible `my_thread` will be destroyed when the main thread ends. Will this cause any problem
  // for the new thread?
  // Answer: No. Because the syste will copy the object `my_thread` to the new thread, instead of using or moving
  // the object `my_thread` to the new thread. The thread t2 will run independently even after `my_thread` is destroyed.
  // It can be verified by the output of the program (to check which construct method is called).
  std::cout << "t2 id: " << t2.get_id() << std::endl;
  if (t2.joinable()) {
    t2.join();
  }

  std::thread lam_thread([]() {  // []: capture list, () : parameter list, {} : function body
    std::cout << "lambda thread id: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "lambda thread end" << std::endl;
  });
  if (lam_thread.joinable()) {
    lam_thread.join();
  }

  std::cout << "main thread end" << std::endl;
  return 0;
}
