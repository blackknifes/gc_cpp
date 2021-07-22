# gc_cpp
# 简单垃圾回收机制实现学习

在C++上实现精确式垃圾回收机制，采用最简单的mark-sweep算法，支持多线程gc。目前已分离出mark线程以及sweep线程。<br>
使用方法:

1.在主线程上构造及初始化GCManager
```cpp

int main()
{
  GCManager manager;
  //...
}
```

2.在想要支持的gc类上继承 GarbageCollected，并实现gcTrace，用以追踪需要gc的成员对象
```cpp
class TestChild: public GarbageCollected
{
};

class Test: public GarbageCollected
{
public:
  virtual void gcTrace(GCVisitor* visitor) override
  {
    //使用visitor追踪需要gc的对象目标
    visitor->visit(m_child);
  }
private:
  GCPtr<TestChild> m_child;
};
```
3.在需要支持gc的线程上构造GCThreadState
```cpp
DWORD CALLBACK ThreadProc(LPVOID pData)
{
  GCThreadState threadState;
}
```

4.在代码中使用GCScope来保证对象的生命周期
```cpp
{
  GCScope scope;
  //do something
}
```

5.在堆栈上使用GCPersist<Type>来记录在堆栈上的根对象，以保证精确gc的正确性
```cpp
GCPersist<Test> pObject = new Test();
```


6.在需要gc时，在任意支持gc的线程上发起gc
```cpp
GCManager::GetGlobal()->gc(); //发起gc，实际gc将在mark 以及 sweep线程上执行
```

7.退出时，等待所有gc线程结束

