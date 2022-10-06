#ifndef PARALLEL_ACCUMULATE
#define PARALLEL_ACCUMULATE
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <thread>
#include <vector>

template <typename Iterator, typename T>
struct accumulate_block {
  void operator()(Iterator first, Iterator last, T& result) { result = std::accumulate(first, last, result); }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
  unsigned long const length = std::distance(first, last);
  if (!length) return init;  // If the range is empty, return the initial value.
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;  // Round up to get the number of threads.
  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2,
               max_threads);  // If the number of threads is greater than the number of hardware threads, limit the
                              // number of threads to the number of hardware threads.
  unsigned long const block_size = length / num_threads;
  std::vector<T> results(num_threads);
  std::vector<std::thread> threads(num_threads - 1);
  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); ++i) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
    block_start = block_end;
  }
  accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
  return std::accumulate(results.begin(), results.end(), init);
}

#endif /* PARALLEL_ACCUMULATE */
