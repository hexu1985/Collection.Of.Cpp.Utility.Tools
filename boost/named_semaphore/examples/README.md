### posix信号量 示例

参考《UNIX网络编程 卷2》，第10章 Posix信号量

**创建信号量**

源码：semcreate.cpp

使用示例：

```
$ ./semcreate --helpshort
semcreate:
usage: ./semcreate [--check_exists] [--name NAME] [--inital_value INITAL_VALUE]

create semaphore


  Flags from semcreate.cpp:
    -check_exists (check semaphore already exists) type: bool default: false
    -inital_value (inital value) type: uint32 default: 1
    -name (semaphore name) type: string default: "sem_test"
$ ls /dev/shm/
$ ./semcreate --name "test" --initial_value 3
$ ls /dev/shm/
sem.test
$ ./semcreate --name "test" --initial_value 3 --check_exists
terminate called after throwing an instance of 'std::system_error'
  what():  sem_open error for test: File exists
已放弃 (核心已转储)
```

**删除信号量**

源码：semunlink.cpp

```
$ ./semunlink --helpshort
semunlink:
usage: ./semunlink [--name NAME]

remove semaphore


  Flags from semunlink.cpp:
    -name (semaphore name) type: string default: "sem_test"
$ ./semcreate --name "test" --initial_value 3
$ ls /dev/shm/
sem.test
$ ./semunlink --name "test"
$ ls /dev/shm/
$ ./semunlink --name "test"
terminate called after throwing an instance of 'std::system_error'
  what():  sem_unlink error: No such file or directory
已放弃 (核心已转储)
```

