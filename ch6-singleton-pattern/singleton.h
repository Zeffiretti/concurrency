#ifndef CH6_SINGLETON_PATTERN_SINGLETON
#define CH6_SINGLETON_PATTERN_SINGLETON

class Singleton {
 public:
  static Singleton& getInstance() {
    if (instance_ == nullptr) {
      instance_ = new Singleton();
    }
    return *instance_;
  }

 private:
  Singleton() = default;
  ~Singleton() = default;
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;
  static Singleton* instance_;
};
Singleton* Singleton::instance_ = nullptr;

#endif /* CH6_SINGLETON_PATTERN_SINGLETON */
