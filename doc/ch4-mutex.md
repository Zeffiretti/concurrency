# 互斥量概念、用法及示例
[toc]

## 互斥量的概念
操作数据时，某个线程用代码把共享数据锁住，其他线程在访问该数据时，需要等待锁被释放后才能访问。此时该线程会将数据锁住，其他线程无法访问该数据，直到该线程释放锁。
互斥量是一个类对象，用于保护共享数据，保证数据在同一时刻只能被一个线程访问。
多个线程尝试用 `lock()` 方法锁住同一个互斥量时，只有一个线程能够成功锁住，其他线程会被阻塞（进入等待队列），直到互斥量被释放。
使用互斥量时，需要注意判别哪些代码需要被互斥量保护，哪些代码不需要被保护。如果不小心将不需要被保护的代码也放在互斥量保护的范围内，可能会导致死锁。

## 互斥量的用法
1. 创建互斥量对象
2. 锁住互斥量 (`std::lock()`)
3. 释放互斥量 (`std::unlock()`)

注意：互斥量的创建、锁住、释放都需要在同一个线程中完成。`std::lock()` 与 `std::unlock` 的调用次数必须相同。忘记 `std::unlock` 的错误很难排查。
为了避免忘记 `std::unlock`，可以使用 `std::lock_guard` 类，它的构造函数会自动调用 `std::lock()`，析构函数会自动调用 `std::unlock()`。

## 死锁
### 死锁产生
当多个线程同时尝试锁住多个互斥量时，可能会导致死锁。例如，线程 A 锁住了互斥量 a，然后尝试锁住互斥量 b，但是线程 B 正在等待互斥量 a 被释放，而线程 B 正在等待互斥量 b 被释放，这样就导致了死锁。

如：现有金锁、银锁；两个线程A、B
1. 线程A执行的时候，这个线程先锁金锁，把金锁lock()成功后，然后锁银锁；
   > 出现上下文切换
2. 线程B执行了，这个线程先锁银锁，因为银锁还没被锁，所以银锁lock成功，线程B要去lock金锁…
   > 这样死锁就产生了。
3. 线程A因为拿不到银锁头，流程走不下去。
4. 线程B因为拿不到金锁头，流程走不下去。

### 死锁的解决方案
只要保证每个线程在锁互斥量时，按照相同的顺序锁住互斥量即可。例如，线程 A 锁住了互斥量 a，然后尝试锁住互斥量 b，线程 B 锁住了互斥量 b，然后尝试锁住互斥量 a。

### `std::lock()` 函数模板
`std::lock()` 函数模板可以用来锁住多个互斥量，它会按照互斥量的地址顺序锁住互斥量，这样就可以避免死锁。如果互斥量中有一个已经被锁住了，`std::lock()` 会阻塞当前线程，直到所有互斥量都被锁住。

### `std::lock_guard` 的 `std::adopt()` 参数
`std::lock_guard` 类的构造函数可以接受一个 `std::adopt_lock` 参数，这个参数的作用是告诉 `std::lock_guard` 类不要再调用 `std::lock()` 函数，而是直接使用已经被锁住的互斥量。

在实际工程中，一般避免同时对多个互斥量上锁。


> ### 死锁的条件
> 一般来说，要出现死锁问题需要满足以下条件：
> 
> 1. 互斥条件：一个资源每次只能被一个线程使用。
> 2. 请求与保持条件：一个进程因请求资源而阻塞时，对已获得的资源保持不放。
> 3. 不剥夺条件：进程已获得的资源，在未使用完之前，不能强行剥夺。
> 4. 循环等待条件：若干进程之间形成一种头尾相接的循环等待资源关系。
> 
> 只要破坏死锁 4 个必要条件之一中的任何一个，死锁问题就能被解决。
> 
> ### 死锁解决方案
> 死锁是由四个必要条件导致的，所以一般来说，只要破坏这四个必要条件中的一个条件，死锁情况就应该不会发生。
> 
> 如果想要打破互斥条件，我们需要允许进程同时访问某些资源，这种方法受制于实际场景，不太容易实现条件；
> 打破不可抢占条件，这样需要允许进程强行从占有者那里夺取某些资源，或者简单一点理解，占有资源的进程不能再申请占有其他资源，必须释放手上的资源之后才能发起申请，这个其实也很难找到适用场景；
> 进程在运行前申请得到所有的资源，否则该进程不能进入准备执行状态。这个方法看似有点用处，但是它的缺点是可能导致资源利用率和进程并发性降低；
> 避免出现资源申请环路，即对资源事先分类编号，按号分配。这种方式可以有效提高资源的利用率和系统吞吐量，但是增加了系统开销，增大了进程对资源的占用时间。
> 如果我们在死锁检查时发现了死锁情况，那么就要努力消除死锁，使系统从死锁状态中恢复过来。消除死锁的几种方式：
> 
> 1. 最简单、最常用的方法就是进行系统的重新启动，不过这种方法代价很大，它意味着在这之前所有的进程已经完成的计算工作都将付之东流，包括参与死锁的那些进程，以及未参与死锁的进程；
> 2. 撤消进程，剥夺资源。终止参与死锁的进程，收回它们占有的资源，从而解除死锁。这时又分两种情况：一次性撤消参与死锁的全部进程，剥夺全部资源；或者逐步撤消参与死锁的进程，逐步收回死锁进程占有的资源。一般来说，选择逐步撤> 消的进程时要按照一定的原则进行，目的是撤消那些代价最小的进程，比如按进程的优先级确定进程的代价；考虑进程运行时的代价和与此进程相关的外部作业的代价等因素；
> 3. 进程回退策略，即让参与死锁的进程回退到没有发生死锁前某一点处，并由此点处继续执行，以求再次执行时不再发生死锁。虽然这是个较理想的办法，但是操作起来系统开销极大，要有堆栈这样的机构记录进程的每一步变化，以便今后的回> 退，有时这是无法做到的。