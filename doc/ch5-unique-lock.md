# unique lock 详解
[toc]

## `unique_lock` 取代 `lock_guard`
`std::unique_lock` 是一个类模板。它的构造函数接受一个 `mutex` 对象的引用，然后调用 `mutex` 的 `lock()` 方法。`unique_lock` 的析构函数会调用 `mutex` 的 `unlock()` 方法。`unique_lock` 的构造函数和析构函数都是 `noexcept` 的，所以它可以用在异常处理中。

与 `lock_guard` 相比，`unique_lock` 使用更加灵活。可以在构造函数中指定 `std::defer_lock`，这样就不会调用 `mutex` 的 `lock()` 方法。这样就可以在构造函数中指定 `mutex` 的 `lock()` 方法的参数。比如，可以指定 `std::try_to_lock`，这样就可以尝试获取锁，如果获取不到就不会阻塞。

## `unique_lock` 的构造函数
`unique_lock` 的构造函数有两个版本，一个是接受一个 `mutex` 对象的引用，另一个是接受一个 `defer_lock_t` 对象的引用。`defer_lock_t` 是一个空类，它的唯一作用就是用来区分这两个构造函数。

## `unique_lock` 的第二个参数
`unique_lock` 的第二个参数是一个 `std::defer_lock_t` 对象的引用。这个参数的作用是告诉 `unique_lock` 不要在构造函数中调用 `mutex` 的 `lock()` 方法。这样就可以在构造函数中先初始化 `unique_lock`，然后在需要的时候再调用 `lock()` 方法。

## `unique_lock` 的成员函数
1. `lock()` 方法
    `unique_lock` 的 `lock()` 方法会调用 `mutex` 的 `lock()` 方法。如果 `unique_lock` 的构造函数中指定了 `std::defer_lock`，那么就需要在需要的时候调用 `lock()` 方法。
2. `unlock()` 方法
    `unique_lock` 的 `unlock()` 方法会调用 `mutex` 的 `unlock()` 方法。如果 `unique_lock` 的构造函数中指定了 `std::defer_lock`，那么可以在需要的时候调用 `unlock()` 方法。在 `unique_lock` 对象析构时，也会自动调用 `mutex` 的 `unlock()` 方法。
3. `try_lock()` 方法
    类似 `std::try_to_lock`，`unique_lock` 的 `try_lock()` 方法会尝试获取锁，如果获取不到就**不会阻塞**。如果获取到了锁，就返回 `true`，否则返回 `false`。
4. `release()` 方法
    `unique_lock` 的 `release()` 方法会释放 `mutex` 的所有权。也就是说，`unique_lock` **不再管理** `mutex`，也不会在析构时调用 `mutex` 的 `unlock()` 方法。`release()` 方法返回 `mutex` 的指针，这样就可以在需要的时候手动调用 `mutex` 的 `unlock()` 方法。

## `try_to_lock` 和 `adopt_lock`
`unique_lock` 的第二个参数还可以是 `std::try_to_lock_t` 或 `std::adopt_lock_t` 对象的引用。`try_to_lock_t` 和 `adopt_lock_t` 都是空类，它们的唯一作用就是用来区分这两个构造函数。
使用 `try_to_lock` 时，`unique_lock` 的构造函数会调用 `mutex` 的 `try_lock()` 方法。如果 `mutex` 已经被锁定，那么 `try_lock()` 方法会返回 `false`，`unique_lock` 的构造函数会抛出一个异常。如果 `mutex` 没有被锁定，那么 `try_lock()` 方法会返回 `true`，`unique_lock` 的构造函数会返回。

## `defer_lock`
`defer_lock` 是一个空类，它的唯一作用就是用来区分 `unique_lock` 的构造函数。`defer_lock` 的构造函数不会调用 `mutex` 的 `lock()` 方法。这样就可以在构造函数中先初始化 `unique_lock`，然后在需要的时候再调用 `lock()` 方法。

## `unique_lock` 的移动语义：所有权转移
`unique_lock` 的移动语义是所有权转移。也就是说，当一个 `unique_lock` 对象被移动赋值给另一个 `unique_lock` 对象时，它会把自己管理的 `mutex` 的所有权转移给另一个 `unique_lock` 对象。这样，原来的 `unique_lock` 对象就不再管理任何 `mutex` 了。
```cpp
std::mutex m1, m2;
// in thread 1
std::unique_lock<std::mutex> lock1(m1, std::defer_lock); // lock1 拥有 m1 的所有权
lock1.lock(); // lock1 管理 m1
std::unique_lock<std::mutex> lock2(std::move(lock1)); // lock2 拥有 m1 的所有权，lock1 不再管理任何 mutex
lock2.unlock(); // lock2 释放 m1 的所有权
```

一般把锁头锁住的代码多少称作锁的粒度。锁的粒度越小，锁的竞争就越激烈，性能就越差。锁的粒度越大，锁的竞争就越小，性能就越好。但是，锁的粒度越大，就越容易出现死锁。