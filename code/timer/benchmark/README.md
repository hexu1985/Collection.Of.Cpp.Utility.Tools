定时器对象
此类表示一个操作应该在等待一定的时间之后运行 --- 相当于一个定时器。 Timer 类是 Thread 类的子类，因此可以像一个自定义线程一样工作。

与线程一样，通过调用 start() 方法启动定时器。而 cancel() 方法可以停止计时器（在计时结束前）， 定时器在执行其操作之前等待的时间间隔可能与用户指定的时间间隔不完全相同。

例如：

```
from threading import  Timer

def hello():
    print("hello, world")

t = Timer(3.0, hello)
t.start()  # after 3 seconds, "hello, world" will be printed
```

class threading.Timer(interval, function, args=None, kwargs=None)
创建一个定时器，在经过 interval 秒的间隔事件后，将会用参数 args 和关键字参数 kwargs 调用 function。如果 args 为 None （默认值），则会使用一个空列表。如果 kwargs 为 None （默认值），则会使用一个空字典。

在 3.3 版更改: 从工厂函数变为类。

cancel()
停止定时器并取消执行计时器将要执行的操作。仅当计时器仍处于等待状态时有效。
