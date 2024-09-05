### posix共享内存 示例

参考《UNIX网络编程 卷2》，第5章 Posix消息队列

**创建消息队列**

源码：mqcreate.cpp

使用示例：

```
$ ./mqcreate --helpshort
mqcreate:
usage: ./mqcreate [--check_exists] [--name NAME] [--queue_max_size QUEUE_MAX_SIZE] [--message_max_size MESSAGE_MAX_SIZE]

create message queue


  Flags from mqcreate.cpp:
    -check_exists (check message queue already exists) type: bool
      default: false
    -message_max_size (max size of a message (in bytes)) type: uint32
      default: 0
    -name (message queue name) type: string default: "/mq_test"
    -queue_max_size (max number of messages allowed on queue) type: uint32
      default: 0
$ ./mqcreate --name "/test" --message_max_size 1024 --queue_max_size 5 
$ ls /dev/mqueue
$ test
$ ./mqcreate --name "/test" --check_exists
terminate called after throwing an instance of 'std::system_error'
  what():  mq_open error for /test: File exists
已放弃 (核心已转储)
```


**删除消息队列**

源码：mqunlink.cpp

```
$ ./mqunlink --helpshort
mqunlink: 
usage: ./mqunlink [--name NAME]

remove message queue


  Flags from mqunlink.cpp:
    -name (message queue name) type: string default: "/mq_test"
$ ./mqcreate --name "/test" --message_max_size 1024 --queue_max_size 5 
$ ls /dev/mqueue
$ test
$ ./mqunlink --name "/test"
$ ls /dev/mqueue/
$ ./mqunlink --name "/test"
terminate called after throwing an instance of 'std::system_error'
  what():  mq_unlink error: No such file or directory
已放弃 (核心已转储)
```

**获取消息队列属性、读写消息队列**

源码：mqgetattr.cpp，mqsend.cpp，mqreceive.cpp

```
$ ./mqcreate --name "/test" --message_max_size 1024 --queue_max_size 5 
$ ./mqgetattr --helpshort
mqgetattr: 
usage: ./mqgetattr [--name NAME]

get attribute of message queue


  Flags from mqgetattr.cpp:
    -name (message queue name) type: string default: "/mq_test"
$ ./mqgetattr --name "/test"
max #msgs = 5, max #bytes/msg = 1024, #currently on queue = 0
$ ./mqsend --helpshort
mqsend: 
usage: ./mqsend [--name NAME] [--message_size MESSAGE_SIZE] [--fill_with CHAR]

send message to queue


  Flags from mqsend.cpp:
    -fill_with (filling message with char) type: string default: "a"
    -message_size (message size (in bytes)) type: uint32 default: 10
    -name (message queue name) type: string default: "/mq_test"
    -priority (message priority) type: uint32 default: 0
$ ./mqsend --name "/test" --fill_with 'x' --priority 3
$ ./mqgetattr --name "/test"
max #msgs = 5, max #bytes/msg = 1024, #currently on queue = 1
$ ./mqsend --name "/test" --fill_with 'y' --priority 7
$ ./mqgetattr --name "/test"
max #msgs = 5, max #bytes/msg = 1024, #currently on queue = 2
$ ./mqreceive --helpshort
mqreceive:
usage: ./mqreceive [--nonblock] [--name NAME]

receive message from queue


  Flags from mqreceive.cpp:
    -dump (dump message) type: bool default: false
    -name (message queue name) type: string default: "/mq_test"
    -nonblock (receive message nonblock from queue) type: bool default: false
$ ./mqreceive --name "/test" --nonblock --dump
read 10 bytes, priority = 7
content[yyyyyyyyyy]
$ ./mqreceive --name "/test" --nonblock --dump
read 10 bytes, priority = 3
content[xxxxxxxxxx]
$ ./mqreceive --name "/test" --nonblock --dump
terminate called after throwing an instance of 'std::system_error'
  what():  mq_receive error: Resource temporarily unavailable
已放弃 (核心已转储)
```

