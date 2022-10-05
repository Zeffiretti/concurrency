#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#define UNUSED(x) (void)(x)

void myPring(const int& i, char* pmybuf) {
  // Even though `i` is a reference, it is **copied** when the thread is created.
  // `i` is not a reference to the original variable in the main thread.
  // But `pmybuf` is a pointer, it is **not copied** when the thread is created.
  // NOTE: please avoid pointer in thread, it is not safe.
  std::cout << i << " " << pmybuf << std::endl;
  return;
}

// a safe way to pass arguments to thread
void myPringSafe(const int& i, const std::string& str) {
  std::cout << i << " " << str << std::endl;
  return;
}

class MThreadTest {
 public:
  MThreadTest(int i) : m_i(i) {
    std::cout << "MThreadTest construct function." << this << " " << std::this_thread::get_id() << std::endl;
  }
  MThreadTest(const MThreadTest& other) : m_i(other.m_i) {
    std::cout << "MThreadTest copy construct function." << this << " " << std::this_thread::get_id() << std::endl;
  }
  // move constructor
  MThreadTest(MThreadTest&& other) : m_i(other.m_i) {
    std::cout << "MThreadTest move construct function." << this << " " << std::this_thread::get_id() << std::endl;
  }
  ~MThreadTest() {
    std::cout << "MThreadTest destruct function." << this << " " << std::this_thread::get_id() << std::endl;
  }
  // operator <<
  friend std::ostream& operator<<(std::ostream& os, const MThreadTest& obj) {
    os << obj.m_i;
    return os;
  }

 private:
  int m_i;
};

void myPrintClass(const int i, const MThreadTest& mtt) {
  std::cout << i << " " << mtt << "thread id is: " << std::this_thread::get_id() << std::endl;
  return;
}

class MTheradRef {
 public:
  int m_i;
  MTheradRef(int i) : m_i(i) {
    std::cout << "MTheradRed construct function." << this << " " << std::this_thread::get_id() << std::endl;
  }
  ~MTheradRef() {
    std::cout << "MTheradRed destruct function." << this << " " << std::this_thread::get_id() << std::endl;
  }
};

void myPringRef(const int i, MTheradRef& mtr) {
  mtr.m_i = i;
  std::cout << i << " " << mtr.m_i << "thread id is: " << std::this_thread::get_id() << std::endl;
  return;
}

void myPrintUniquePtr(const int i, std::unique_ptr<int>& int_ptr) {
  *int_ptr = i;
  std::cout << i << " " << *int_ptr << "thread id is: " << std::this_thread::get_id() << std::endl;
  return;
}

class MThreadMethod {
 public:
  void print(const int i, const std::string& str) {
    std::cout << i << " " << str << "thread id is: " << std::this_thread::get_id() << std::endl;
    return;
  }
};

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);
  std::cout << "main thread starts." << std::endl;
  int m_var = 1;
  int& ref_var = m_var;
  char mybuf[] = "this is a test text.";

  std::thread t(myPring, ref_var, mybuf);  // Is it a value or a reference passed to the thread?
  t.join();  // If we use detach() here, the main thread will end before the new thread ends.
  // t.detach();  // Will it cause memory leak?

  std::thread t_safe(myPringSafe, ref_var,
                     std::string(mybuf));  // Here we use std::string to copy the content of mybuf.
                                           // Before construct the thread, we have a temporary std::string object.
  t_safe.detach();

  int m_i = 12;
  std::cout << "main thread id is: " << std::this_thread::get_id() << std::endl;
  // std::thread t_class(myPrintClass, m_var, m_i);  // Although m_i is an int, the system wiil call the constructor
  // of MThreadTest to create a temporary object.
  // But if the temporary object has not been created before the main
  // thread ends, `m_i` will be destroyed.
  std::thread t_class(myPrintClass, m_var, MThreadTest(m_i));  // Here we create a temporary object before
  // the thread is created.
  t_class.detach();

  MTheradRef mtr(10);
  std::cout << "[main] mtr.m_i is: " << mtr.m_i << std::endl;
  std::thread t_ref(myPringRef, m_var, std::ref(mtr));
  std::cout << "[main] mtr.m_i is: " << mtr.m_i << std::endl;
  t_ref.join();

  std::unique_ptr<int> int_ptr(new int(100));
  std::cout << "[main] int_ptr is: " << *int_ptr << std::endl;
  std::thread t_unique_ptr(myPrintUniquePtr, m_var, std::ref(int_ptr));
  // std::cout << "[main] int_ptr is: " << *int_ptr << std::endl;
  t_unique_ptr.join();

  MThreadMethod mthm;
  std::thread t_method(&MThreadMethod::print, &mthm, m_var, std::string(mybuf));
  t_method.join();

  std::cout << "main thread ends." << std::endl;

  return 0;
}
