### SpinLock自旋锁类

从C++20开始，ATOMIC_FLAG_INIT不再必须，因为 std::atomic_flag 的默认构造函数将其初始化为清除状态。它为与 C 的兼容而保留。

### 参考
C++并发编程实战, 5.2.2章节, <https://github.com/subjam/concurrency-in-action>

[互斥锁与自旋锁（用户态）的性能比较](https://zhuanlan.zhihu.com/p/264848910)
