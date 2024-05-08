### 基于C++标准库实现工作线程类

**简介**

这篇文章将介绍如何基于C++标准库来实现工作线程类。
首先给出我们要实现的工作线程的定义：
我们可以将工作线程类简单的理解为简化的线程池类：只有一个工作线程的线程池类。
或者也可以类比于GUI编程里的eventloop线程。

我们先给出工作线程的基本数据结构和类对象的角色。如下图：

![工作线程](worker_thread.png)

可能有人会问既然工作线程类可以由线程池类简化而成，为啥还要
单独实现工作线程类，我给出的答案很简单：简单，工作线程类的实现更简单；）
在简单的场景中，可以用工作线程类来简化程序的实现时，
单独的工作线程类实现更简单（相比线程池类），并给到更好的可读性（因为类名更具体化）。

其次，由于任务队列和工作线程是1对1的，也就是，工作队列里的任务是串行的（不用加锁），
这时可能有人会说这样一来不就失去并发性了么？其实不然，以GUI编程为例，mainloop里
为了及时响应用户操作和UI渲染，会把文件下载或加载等需要长时间阻塞的IO事件放到单独
一个线程里处理，这种情况工作线程就十分合适，既避免了每次创建线程的开销，又保证了任务队列
自动排队。

**实现**

下面我们通过一个具体多线程例子，来说明工作线程类的实现原理。

我们的原始实例代码是这样的，有两个线程： producer和consumer，
以及一个数据队列连接着两个进程。
- producer创建整数数据并放入数据队列中
- consumer从数据队列中读取整数数据，并调用处理函数处理数据。

由于C++标准库没有线程安全的队列，另外，这里也不想在原始的示例
代码中引入自己实现的线程安全队列，而分心读者，这里就给出了
python版本的原始示例代码，如下：


```python
#!/usr/bin/env python

from random import randint
from time import sleep
from queue import Queue
from threading import Thread

def writeQ(queue, val):
    print('producing object for Q...:', val)
    queue.put(val)

def readQ(queue):
    val = queue.get(1)
    print('consumed object from Q...:', val)
    return val

def mul_two(i):
    print('mul_two({}) is {}'.format(i, i*2))

def producer(queue, loops):
    for i in range(loops):
        writeQ(queue, i)
        sleep(randint(1, 3))

    writeQ(queue, -1)

def consumer(queue, loops):
    while True:
        i = readQ(queue)
        if i < 0: break
        mul_two(i)
        sleep(randint(2, 5))

funcs = [producer, consumer]
nfuncs = range(len(funcs))

def main():
    nloops = randint(2, 5)
    q = Queue(32)

    threads = []
    for i in nfuncs:
        t = Thread(target=funcs[i], args=(q, nloops))
        threads.append(t)

    for i in nfuncs:
        threads[i].start()

    for i in nfuncs:
        threads[i].join()

    print('all DONE')

if __name__ == '__main__':
    main()
```

代码运行的结果如下：

```python
producing object for Q...: 0
consumed object from Q...: 0
mul_two(0) is 0
producing object for Q...: 1
consumed object from Q...: 1
mul_two(1) is 2
producing object for Q...: 2
producing object for Q...: -1
consumed object from Q...: 2
mul_two(2) is 4
consumed object from Q...: -1
all DONE
```

如果你在本地运行的结果和我这里的不一样，也不用担心，
代码中是有随机数的，所以数据量可能不同，两个线程的sleep时间也可能不同（造成某些时序不太一样）。

理解了原始示例代码里的代码逻辑，我们来看看如果改成用WorkerThread来实现，
代码又会长成什么样，这里给出C++版本的（是不是感觉跳跃有点儿大；）)

```cpp
#include <iostream>
#include <thread>
#include <chrono>

#include "random.hpp"
#include "worker_thread.hpp"

void mul_two(int i) {
    std::cout << "mul_two(" << i << ") is " << i*2 << std::endl;
}

void consume(int i) {
    mul_two(i);
    std::this_thread::sleep_for(std::chrono::seconds(randint(2, 5)));
}

void producer(std::shared_ptr<TaskQueue> queue, int loops) {
    for (int i = 0; i < loops; i++) {
        queue->PushTask(&consume, i);
        std::this_thread::sleep_for(std::chrono::seconds(randint(1, 3)));
    }
}

int main() {
    int nloops = randint(2, 5);

    WorkerThread myworker("worker");
    myworker.Start();

    std::thread myproducer(&producer, myworker.GetTaskQueue(), nloops);
    myproducer.join();

    myworker.Stop();
    std::cout << "all DONE" << std::endl;

    return 0;
}
```

有没有很惊奇的感觉？因为相同功能的C++代码竟然比Python的代码还要短；）
我们简单的分析一下，首先C++代码里没有显式的数据队列（因为这里被WorkerThread的任务队列替代了），
另外，也看不到显式的consumer线程了，取而代之的是在producer线程里，把数据和consume函数一起打包
成一个个task，直接丢到WorkerThread的TaskQueue里了，而从TaskQueue里取出任务并执行的代码逻辑
被封装到了WorkerThread的实现里，并且TaskQueue也是线程安全的。
