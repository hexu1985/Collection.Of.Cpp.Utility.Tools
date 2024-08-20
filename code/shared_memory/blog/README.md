## C++封装Posix API之共享内存

本文介绍将Posix共享内存相关API封装C++类的过程。
本文主要分为以下部分：
- Posix共享内存API介绍及示例
- C++封装类的实现以及示例

至于C++封装类的好处与代价，留给读者自己判断;)

### Posix共享内存API介绍及示例

注：这里的API介绍和示例都来自《UNIX网络编程卷2进程间通信(第2版)》这本书。

Posix.1提供了两种在无亲缘关系进程间共享内存区的方法
- 内存映射文件(memory-mapped file)，由open函数打开，由mmap函数把所得到的描述符映射到当前进程空间地址中的一个文件。
- 共享内存区对象(shared-memory object)，由shm_open函数打开一个Posix.1 IPC名字，所返回的描述符由mmap函数映射到当前进程的地址空间。

这两种技术都需要调用mmap，差别在于作为mmap的参数之一的描述符的获取手段：通过open或通过shm_open。


### 参考文档：

- 《UNIX网络编程卷2进程间通信(第2版)》
- 《C++嵌入式开发实例精解》
- [IPC之Posix共享内存详解](https://blog.csdn.net/daiyudong2020/article/details/50500651)
- [Posix共享内存](https://www.cnblogs.com/songhe364826110/p/11530732.html)
