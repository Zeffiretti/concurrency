# 线程传参，detach()与成员函数做线程函数
[toc]

## 传递临时对象作为线程参数
```cpp
void myPrint(const int i, const std::string& str) {
  std::cout << "Thread " << i << " says: " << str << std::endl;
}

int m_var = 1;
int& ref_var = m_var;
char mybuf[] = "this is a test text.";

std::thread t(myPring, ref_var, mybuf);  // Is it a value or a reference passed to the thread?
t.join();
```

> 1. 传递临时对象作为线程参数，会被拷贝到线程内部，因此，如果传递的是引用，那么线程内部的引用指向的是拷贝后的对象，而不是原始对象。
> 2. 如果传递的是指针，那么线程内部的指针指向的是原始对象。
> 3. 为避免主线程退出后，子线程还在运行，导致子线程访问到已经销毁的对象，需要避免传入指针或引用。
> 4. 传递给子线程的变量会构造临时对象，如果非显式调用拷贝构造函数，那么无法保证在主线程结束前该临时对象成功构造，因此尽量避免隐式类型转换。

**如果传递简单的类型，建议使用值传递而不是引用传递，避免对内存的非法引用。
如果传递复杂的类型，建议避免使用隐式类型转换。需要全部都在创建线程时构建出临时对象并在函数参数中用引用传递。**

### 线程id
id 是一个数字，可以用来标识线程，但是不同平台的线程id的类型不同，因此不能直接比较，也不能直接输出。每个线程对应着一个数字，但是这个数字不是固定的，因此不能用来标识线程。

### 临时对象构造时机
如果显式调用拷贝构造函数，那么临时对象会在主线程中构造，如果不显式调用拷贝构造函数，那么临时对象会在子线程中构造。为了保证子线程运行时，临时对象已经构造完成，需要显式调用拷贝构造函数。

## 传递类对象、智能指针作为线程参数 (std::ref)
```cpp
class A {
 public:
  A(int i) : m_i(i) {}
  int m_i;
};

void myPrint(A& a) {
  a.m_i = 2;
  std::cout << "Thread says: " << a.m_i << std::endl;
}

int main() {
  A a(1);

  t.join();
  return 0;
}
```

## 使用类成员函数作为线程函数
```cpp
class A {
 public:
  A(int i) : m_i(i) {}
  void myPrint() {
    std::cout << "Thread says: " << m_i << std::endl;
  }
  int m_i;
};

int main() {
  A a(1);

  t.join();
  return 0;
}
```

