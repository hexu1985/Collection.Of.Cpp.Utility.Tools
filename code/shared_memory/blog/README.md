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

![posix_memory_objects](posix_memory_objects.png)

本文主要介绍的就是基于shm_open相关接口的共享内存技术，具体接口列表如下：
- shm_open
- shm_unlink
- mmap
- munmap
- ftruncate
- fstat
- close

我们接下来分别介绍每个接口的含义：

**shm_open函数**

shm_open用于创建一个新的Posix共享内存对象或打开一个已存在的Posix共享内存对象。

传递给shm_open函数的名字参数随后由希望共享该内存区的任何其他进程使用。

```c
SHM_OPEN(3)                Linux Programmer's Manual               SHM_OPEN(3)
NAME
       shm_open,  shm_unlink  -  create/open  or  unlink  POSIX  shared memory
       objects
SYNOPSIS
       #include <sys/mman.h>
       #include <sys/stat.h>        /* For mode constants */
       #include <fcntl.h>           /* For O_* constants */
       int shm_open(const char *name, int oflag, mode_t mode);

       Link with -lrt.
```

返回：若成功则非负描述符，若出错则为-1。

shm_open参数说明：

- oflag参数不能设置O_WRONLY标志
- 和mq_open、sem_open不同，shm_open的mode参数总是必须指定，当指定了O_CREAT标志时，mode为用户权限位，否则将mode设为0

shm_open的返回值是一个描述符，它随后用作mmap的第五个参数fd。


**shm_unlink函数**

shm_unlink用于从系统中删除一个Posix共享内存对象。

删除一个名字不会影响对于其底层支撑对象的现有引用，直到对于该对象的引用全部关闭为止。

```c
SHM_OPEN(3)                Linux Programmer's Manual               SHM_OPEN(3)
NAME
       shm_open,  shm_unlink  -  create/open  or  unlink  POSIX  shared memory
       objects
SYNOPSIS
       #include <sys/mman.h>
       #include <sys/stat.h>        /* For mode constants */
       #include <fcntl.h>           /* For O_* constants */
       int shm_unlink(const char *name);

       Link with -lrt.
```

返回：若成功则为0，若出错则为-1。


**mmap函数**

mmap函数把一个文件或一个Posix共享内存对象映射到调用进程的地址空间，使用该函数有三个目的：

- 使用普通文件以提供内存映射IO
- 使用特殊文件以提供匿名内存映射
- 使用Posix共享内存对象以提供Posix共享内存区

本文中主要用于：使用Posix共享内存对象以提供Posix共享内存区

```c
MMAP(2)                    Linux Programmer's Manual                   MMAP(2)

NAME
       mmap, munmap - map or unmap files or devices into memory

SYNOPSIS
       #include <sys/mman.h>

       void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);

       See NOTES for information on feature test macro requirements.
```

返回：若成功则为被映射区的起始地址，若出错则为MAP_FAILED。

mmap参数解析：

- addr指定映射内存的起始地址，通常设为NULL，让内核自己决定起始地址
- length是被映射到调用进程地址空间中的字节数，它从被映射文件fd开头起第offset个字节处开始算，offset通常设为0，下图展示了这个映射关系
- prot指定对映射内存区的保护，通常设为PROT_READ | PROT_WRITE
    + PORT_READ     -> 可读
    + PORT_WRITE    -> 可写
    + PORT_EXEC     -> 可执行
    + PORT_NONE     -> 数据不可访问
- flags必须在MAP_SHARED和MAP_PRIVATE这两个标志中选择指定一个，进程间共享内存需要使用MAP_SHARED
    + MAP_SHARED  -> 变动是共享的
    + MAP_PRIVATE  -> 变动是私自的
    + MAP_FIXED        -> 准确的解析addr参数
- 可移植的代码应把addr设为NULL，并且flags不指定MAP_FIXED

![memory_mappedfile](memory_mappedfile.png)

mmap成功返回后，fd参数可以关闭。该操作对由于mmap建立的映射关系没有影响。








### 参考文档：

- 《UNIX网络编程卷2进程间通信(第2版)》
- 《C++嵌入式开发实例精解》
- [IPC之Posix共享内存详解](https://blog.csdn.net/daiyudong2020/article/details/50500651)
- [Posix共享内存](https://www.cnblogs.com/songhe364826110/p/11530732.html)
