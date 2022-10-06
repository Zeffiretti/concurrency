# 单例设计模式中的数据共享
[toc]

## 设计模式(Design Pattern)概论
设计模式（Design pattern）代表了最佳的实践，通常被有经验的面向对象的软件开发人员所采用。设计模式是**软件开发人员在软件开发过程中面临的一般问题的解决方案**。这些解决方案是众多软件开发人员经过相当长的一段时间的试验和错误总结出来的。

设计模式是一套被反复使用的、多数人知晓的、经过分类编目的、代码设计经验的总结。使用设计模式是为了重用代码、让代码更容易被他人理解、保证代码可靠性。

## 单例设计模式
单例设计模式（Singleton Design Pattern）是一种常用的软件设计模式。在它的核心结构中只包含一个被称为单例的特殊类。通过单例模式可以保证系统中，应用该模式的一个类只有一个实例。即**一个类只有一个对象实例**。

单例类写法
```cpp
class Singleton {
 public:
  static Singleton* getInstance() {
    if (instance == nullptr) {
	    instance = new Singleton();
    }
    return instance;
  }
 private:
  Singleton() {} // 构造函数私有化
  static Singleton* instance;
};
Singleton* Singleton::instance = nullptr;

// in main()
Singleton* s1 = Singleton::getInstance();
```