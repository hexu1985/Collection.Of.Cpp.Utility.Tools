import threading
import datetime

def say_hello():
    print("{}: Hello, World!".format(datetime.datetime.now()))

# 创建一个 Timer，5 秒后执行 say_hello 函数
timer = threading.Timer(5.0, say_hello)

# 启动定时器
timer.start()

# 在定时器触发前取消
print("{}: Timer started, but we will cancel it.".format(datetime.datetime.now()))
timer.cancel()

print("{}: Timer cancelled.".format(datetime.datetime.now()))
