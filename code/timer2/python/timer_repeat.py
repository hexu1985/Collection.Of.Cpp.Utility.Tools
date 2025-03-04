import threading
import datetime

def repeat_hello():
    print("{}: Hello, World!".format(datetime.datetime.now()))
    # 重新启动定时器
    timer = threading.Timer(2.0, repeat_hello)
    timer.start()

# 创建一个 Timer，2 秒后执行 repeat_hello 函数
timer = threading.Timer(2.0, repeat_hello)

# 启动定时器
timer.start()

print("{}: Repeating timer started...".format(datetime.datetime.now()))
