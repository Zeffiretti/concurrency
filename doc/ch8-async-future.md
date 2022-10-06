# std::async 与 std::future

## `std::async` 详解
`std::async` 是一个函数模板，用来启动一个异步任务。它的第一个参数是一个可调用对象，它的第二个参数是可调用对象的参数。`std::async` 的返回值是一个 `std::future` 对象，通过 `std::future` 对象可以获取异步任务的返回值。通过调用 `std::future` 对象的成员函数 `get` 可以获取异步任务的返回值，如果异步任务还没有完成，`get` 函数会阻塞当前线程，直到异步任务完成。`std::async` 的第三个参数是一个 `std::launch` 枚举值，它的默认值为 `std::launch::async | std::launch::deferred`，表示异步任务可以同步执行，也可以异步执行。如果第三个参数为 `std::launch::async`，表示异步任务必须异步执行；如果第三个参数为 `std::launch::deferred`，表示异步任务必须同步执行。

`std::async` 的 `policy` 参数：
- `std::launch::async`：异步启动，在调用std::async()时创建一个新的线程以异步调用函数，并返回future对象；
- `std::launch::deferred`：延迟启动，在调用std::async()时不创建线程，直到调用了future对象的get()或wait()方法时，才创建线程；此时创建的线程与调用get()或wait()的线程是同一个线程；（也就是实际并未创建新的线程，而是在调用get()或wait()的线程中调用函数）；
- `std::launch::any = std::launch::async | std::launch::deferred`：异步或延迟启动，由系统决定。其工作机制为：如果std::async()调用时，当前线程不是主线程，则创建一个新的线程以异步调用函数；否则，延迟启动，即在调用get()或wait()时，创建一个新的线程以异步调用函数。


## `package_task` 详解：打包任务，把任务包装起来
`package_task` 是一个模板类，它的模板参数是一个可调用对象。`package_task` 的构造函数接受一个可调用对象，它的成员函数 `operator()` 接受可调用对象的参数。`package_task` 的成员函数 `get_future` 返回一个 `std::future` 对象，通过 `std::future` 对象可以获取异步任务的返回值。通过调用 `std::future` 对象的成员函数 `get` 可以获取异步任务的返回值，如果异步任务还没有完成，`get` 函数会阻塞当前线程，直到异步任务完成。`package_task` 的成员函数 `operator()` 启动异步任务，它的参数是可调用对象的参数。`package_task` 的成员函数 `valid` 返回 `true`，表示异步任务已经启动，`false` 表示异步任务还没有启动。

`package_task` 的成员函数 `reset` 可以重置 `package_task` 对象，使其可以再次启动异步任务。`package_task` 的成员函数 `swap` 可以交换两个 `package_task` 对象的状态。