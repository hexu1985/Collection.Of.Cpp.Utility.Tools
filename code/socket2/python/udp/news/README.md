### 测试方式

在终端1的中执行
```
$ python3 news_receiver.py 239.255.0.1 9999
```

在终端2的中执行
```
$ python3 news_receiver.py 239.255.0.1 9999
```

在终端3的中执行
```
$ python3 news_sender.py 239.255.0.1 9999
```

最终，在终端1、2的标准输出会显示结果
