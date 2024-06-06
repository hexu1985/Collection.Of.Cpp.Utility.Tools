### SpinLock自旋锁类

优化：
由于用户态不能关闭调度，使临界区内任务尽快做完。
故系统可能会在临界区调度到其他线程，最坏的情况是其他线程不停的尝试获取这个锁，但因为获取到这个锁的线程还在等待下一次调度，
导致白白浪费宝贵的CPU时间，故我们可以在lock()函数中插入一条std::this_thread::yield()防止这种情况。

### 参考
C++并发编程实战, 5.2.2章节, <https://github.com/subjam/concurrency-in-action>

[互斥锁与自旋锁（用户态）的性能比较](https://zhuanlan.zhihu.com/p/264848910)
