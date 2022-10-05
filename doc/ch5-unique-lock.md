# unique lock 详解
[toc]

## `unique_lock` 取代 `lock_guard`
`std::unique_lock` 是一个类模板。它的构造函数接受一个 `mutex` 对象的引用，然后调用 `mutex` 的 `lock()` 方法。`unique_lock` 的析构函数会调用 `mutex` 的 `unlock()` 方法。`unique_lock` 的构造函数和析构函数都是 `noexcept` 的，所以它可以用在异常处理中。

与 `lock_guard` 相比，`unique_lock` 使用更加灵活。

## `unique_lock` 的构造函数
`unique_lock` 的构造函数有两个版本，一个是接受一个 `mutex` 对象的引用，另一个是接受一个 `defer_lock_t` 对象的引用。`defer_lock_t` 是一个空类，它的唯一作用就是用来区分这两个构造函数。

## `unique_lock` 的第二个参数
`unique_lock` 的第二个参数是一个 `std::defer_lock_t` 对象的引用。这个参数的作用是告诉 `unique_lock` 不要在构造函数中调用 `mutex` 的 `lock()` 方法。这样就可以在构造函数中先初始化 `unique_lock`，然后在需要的时候再调用 `lock()` 方法。

## `try_to_lock` 和 `adopt_lock`
`unique_lock` 的第二个参数还可以是 `std::try_to_lock_t` 或 `std::adopt_lock_t` 对象的引用。`try_to_lock_t` 和 `adopt_lock_t` 都是空类，它们的唯一作用就是用来区分这两个构造函数。
使用 `try_to_lock` 时，`unique_lock` 的构造函数会调用 `mutex` 的 `try_lock()` 方法。如果 `mutex` 已经被锁定，那么 `try_lock()` 方法会返回 `false`，`unique_lock` 的构造函数会抛出一个异常。如果 `mutex` 没有被锁定，那么 `try_lock()` 方法会返回 `true`，`unique_lock` 的构造函数会返回。

## `defer_lock`
`defer_lock` 是一个空类，它的唯一作用就是用来区分 `unique_lock` 的构造函数。`defer_lock` 的构造函数不会调用 `mutex` 的 `lock()` 方法。这样就可以在构造函数中先初始化 `unique_lock`，然后在需要的时候再调用 `lock()` 方法。