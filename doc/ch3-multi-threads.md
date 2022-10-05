# 创建多个线程、数据共享问题分析及案例代码
[toc]

## 创建多个线程
```cpp
void myPrint(int seconds) {
  std::cout << "[" << seconds << "]"
            << "thread id: " << std::this_thread::get_id() << " starts." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
  std::cout << "[" << seconds << "]"
            << "thread id: " << std::this_thread::get_id() << " ends." << std::endl;
}

int main() {
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back(std::thread(myPrint, i));
  }
  for (auto &t : threads) {
    t.join();
  }
  return 0;
}
```
> 1. 多线程的执行顺序是不确定的，因此输出结果也是不确定的，与操作系统的调度有关。
> 2. 主线程会等待子线程执行完毕后才会结束。
> 3. 将子线程对象放入容器中，便于对大量线程进行管理。

## 数据共享问题
### 只读数据
安全稳定，不需要特殊的共享处理手段。
### 有读有写的数据
1. 读写分离
  读数据时不允许写数据，写数据时不允许读数据。
  一个线程写时，其他线程只能读不能写，一个线程读时，其他线程只能读不能写。
  **写的操作分为很多小步，由于随意的任务切换，导致各种异常的情况发生。**
2. 读写互斥
  读写互斥，即读写互斥，写写互斥，读读不互斥。
### 数据共享
典型案例：窗口售票系统