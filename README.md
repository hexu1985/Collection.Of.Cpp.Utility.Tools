## 一个简单的C++工具库, 基于C++11标准

### 工具类或函数列表:

- [TaskQueue](include/TaskQueue.hpp): 一个线程安全的任务队列类
- [WorkerThread](include/WorkerThread.hpp): 一个处理任务队列的工作线程类
- [SpinLock](include/SpinLock.hpp): 一个自旋锁类
- MTQueue: 一个多线程版本的队列类(MTQueue.hpp)
- [Stopwatch](include/Stopwatch.hpp)、[BasicStopwatch](include/BasicStopwatch.hpp)、[TimerBaseChrono](include/TimerBaseChrono.hpp): 一个计时器类
- [MemoryPool](include/MemoryPool.hpp): 固定大小对象的内存池
- [BulkMemoryPool](include/BulkMemoryPool.hpp): 固定大小的内存池
- [ByteMemoryPool](include/ByteMemoryPool.hpp): 可变大小的内存池
- [MTMemoryPool](include/MTMemoryPool.hpp): 一个支持多线程的内存池
- [Timer](Timer.hpp): 一个基于thread的超简单的定时器类
- [dump, dump_ascii, dump_hex](include/dump_functions.hpp): dump数据的函数
- [DumpHelper](include/DumpHelper.hpp): dump数据的帮助类
- [RCPtr](include/RCPtr.hpp)、[RCObject](include/RCObject.hpp): 侵入式引用计数指针类
- [RCIPtr](include/RCIPtr.hpp)、[RCObject](include/RCObject.hpp): 引用计数指针类
- [Singleton](include/Singleton.hpp): 一个单件类模板

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
