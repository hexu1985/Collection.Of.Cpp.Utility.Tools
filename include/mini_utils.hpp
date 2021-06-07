/*! \mainpage
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
*/
#ifndef MINI_UTILS_INC
#define MINI_UTILS_INC
#include "WorkerThread.hpp"
#include "TaskQueue.hpp"
#include "MemoryPool.hpp"
#include "MTQueue.hpp"
#include "error.hpp"
#include "MTMemoryPool.hpp"
#include "DumpHelper.hpp"
#include "TimerBaseChrono.hpp"
#include "RCObject.hpp"
#include "SpinLock.hpp"
#include "Timer.hpp"
#include "ByteMemoryPool.hpp"
#include "BulkMemoryPool.hpp"
#include "RCIPtr.hpp"
#include "BasicStopwatch.hpp"
#include "Singleton.hpp"
#include "Stopwatch.hpp"
#include "RCPtr.hpp"
#include "DelayTaskQueue.hpp"
#include "dump_functions.hpp"
#endif
