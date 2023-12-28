### 基于C++标准库实现定时器Timer类

接口类似于Python标准库的threading.Timer 

- [Python标准库的threading.Timer的使用demo](benchmark)
- [基于std::thread的实现版本(精度为秒)](recipe-01)
- [基于std::mutex和std::condition_variable实现版本(精度为秒)](recipe-02)
- [基于std::thread的实现版本(精度为微秒)](recipe-01)
- [基于std::mutex和std::condition_variable实现版本(精度为为微秒)](recipe-02)


参考代码: Posix多线程程序设计（Programming with POSIX Threads） 1.5.3章节 3.3.4章节
alarm_thread.c alarm_cond.c
