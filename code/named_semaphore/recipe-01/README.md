### posix信号量 示例

参考《UNIX网络编程 卷2》，第10章 Posix信号量

**创建信号量**

源码：semcreate.cpp

使用示例：

```
$ ./semcreate --helpshort
semcreate:
usage: ./semcreate [--check_exists] [--name NAME] [--initial_value INITIAL_VALUE]

create semaphore


  Flags from semcreate.cpp:
    -check_exists (check semaphore already exists) type: bool default: false
    -initial_value (initial value) type: uint32 default: 1
    -name (semaphore name) type: string default: "sem_test"
$ ls /dev/shm/
$ ./semcreate --name "sem_test" --initial_value 3
$ ls /dev/shm/
sem_test
$ ./semgetvalue --name "sem_test"
value = 3
$ ./semcreate --name "sem_test" --check_exists --initial_value 3
terminate called after throwing an instance of 'std::system_error'
  what():  shm_open error for sem_test: File exists
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
$ ./semcreate --name "sim_test" --initial_value 3
$ ls /dev/shm/
sem_test
$ ./semunlink --name "sim_test"
$ ls /dev/shm/
```

