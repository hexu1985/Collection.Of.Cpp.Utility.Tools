## 一个简单的C++工具库, 基于C++11标准

### 工具类或函数列表:

- TaskQueue: 一个线程安全的任务队列类(TaskQueue.hpp)
- WorkerThread: 一个处理任务队列的工作线程类(WorkerThread.hpp)
- SpinLock: 一个自旋锁类(SpinLock.hpp)
- MTQueue: 一个多线程版本的队列类(MTQueue.hpp)
- Stopwatch: 一个计时器类(BasicStopwatch.hpp, TimerBaseChrono.hpp, Stopwatch.hpp)
- MemoryPool: 固定大小对象的内存池(MemoryPool.hpp)
- BulkMemoryPool: 固定大小的内存池(BulkMemoryPool.hpp)
- ByteMemoryPool: 可变大小的内存池(ByteMemoryPool.hpp)
- MTMemoryPool: 一个支持多线程的内存池(MTMemoryPool.hpp)
- Timer: 一个基于thread的超简单的定时器类(Timer.hpp)
- dump, dump_ascii, dump_hex: dump数据的函数(dump_functions.hpp)
- RCPtr: 侵入式引用计数指针类(RCPtr.hpp)
- RCIPtr: 引用计数指针类(RCIPtr.hpp, RCObject.hpp)

### 编译项目：

```shell
$ cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release  
$ cmake --build build
```

### API文档：

API文档是通过doxygen生成的，需要事先安装doxygen，然后通过如下命令生成html格式的API文档：

```shell
$ cd doc
$ ./build_doc.sh
```
