### C++性能优化之内存池

本篇文章不会一上来就介绍内存池的实现，也不会上来就哔哔内存池是如何提高性能，以及
“通用”内存管理为什么有额外开销，随便找本性能优化的书，就能看到很多类似的阐述。

本文旨在站在科学验证的角度，结合工程实践，以内存池的实现为题，介绍一个完整性能优化的过程和该有的步骤。

我们从性能优化的第一步开始：测试。这里的测试有如下几个目的：
- 发现性能的确不行：给出实际的时延或吞吐量，结合期望的时延或吞吐量，证明的确需要优化。
  这里引用高纳德老爷子那句名言：
  ```
  我们应当忘记小的性能改善，百分之九十七的情况下，过早优化都是万恶之源。
        ——高德纳
  ```
  只有通过测试数据，定量（而不是定性）分析，才能知道合时才不是过早。
- 找到性能热点：给出时延占比，找到真正需要优化的函数或代码段。
- 获取基准测试数据：作为后续优化的基准，量化后续优化的性能提升，已经判断何时该停下继续优化的脚步。


我们这篇文章就是介绍内存优化的，所以我们会给出第一步的性能测试的完整例子，但跳过第一条（发现性能的确不行），
我们假设我们的确需要优化内存分配管理这块，但第二条和第三条还是会通过测试得出结论，证明性能热点的确在通用
的内存管理部分（标准库的new、delete以及malloc和free函数）。

**基准测试**

话已够多，还是先上代码吧（代码实现主要来自《Efficient C++: Performance Programming Techniques》，中文版叫《提高C++性能的编译技术》）。

假设我们有一个表示有理数的类，定义在rational.hpp头文件里：

```cpp
#pragma once

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

private:
	int n;	// Numerator
	int d;	// Denominator
};
```

假设我们要反复申请和释放大量Rational对象，下面就给出基准测试代码: example.cpp

```cpp
#include <iostream>
#include <chrono>

#include "rational.hpp"

using namespace std;

int main()
{
	Rational* array[5000];

	// 此处开始计时
	auto start = chrono::steady_clock::now();

	for (int j = 0; j < 10000; j++) {
		for (int i = 0; i < 5000; i++) {
			array[i] = new Rational(i);
		}
		for (int i = 0; i < 5000; i++) {
			delete array[i];
		}
	}

	// 此处停止计时
	auto end = chrono::steady_clock::now();

	cout << "use time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << " ms" << endl;

	return 0;
}
```

完整的工程链接在：[benchmark](https://github.com/hexu1985/Collection.Of.Cpp.Utility.Tools/tree/master/code/memory_pool/benchmark)

编译并运行代码：

```shell
$ clang++ -g -O3 -mavx2 -Wall -pedantic    example.cpp   -o example
$ ./example 
use time: 519 ms
```

单看这个实例运行输出的时延值，其实毫无意义，我们接下来先来找出热点函数，通过linux的perf性能分析器，
具体命令如下：

```shell
$ sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"
$ perf record ./example
$ perf report --stdio
```

我们就会得到如下输出：

![benchmark perf](benchmark_perf.png)

从这个结果来看，我们可以发现，90%的时间都花在了new和delete以及它们底层调用的malloc和free相关的内存管理函数上。
有了这个测试结果，我们才有必要进行优化内存管理和实现内存池。
当然，这个测试代码是量身定做的，但实际项目中，通过linux性能分析工具perf发现性能热点的套路还是可以复用的。

接下来我们要测试具体的时延，作为性能优化的基准，虽然example.cpp中基于chrono::steady_clock的计时可以作为时延数据，
但我这里还是给出用更加专业的工具进行测试的结果：基于Google Benchmark库，Benchmark库的安装和使用这里不做介绍，
不熟悉的同学可以自行去网上搜索资料。

我们将example.cpp稍微改造改造，得出使用Google Benchmark库测试的程序：benchmark.cpp

```cpp
#include <memory>

#include "rational.hpp"
#include "benchmark/benchmark.h"

void new_delete_rational(Rational* array[], int times) {
    for (int i = 0; i < times; i++) {
        array[i] = new Rational(i);
    }
    for (int i = 0; i < times; i++) {
        delete array[i];
    }
}

void BM_memory_alloc(benchmark::State& state) {
    int N = state.range(0);
    std::unique_ptr<Rational* []> array(new Rational* [N]);
    for (auto _ : state) {
        new_delete_rational(array.get(), N);
    }
    state.SetItemsProcessed(N*state.iterations());
}

#define ARGS ->Arg(5000)

BENCHMARK(BM_memory_alloc) ARGS;

BENCHMARK_MAIN();
```

编译并运行代码：

```shell
$ clang++ -o benchmark benchmark.cpp -g -O3 -mavx2 -Wall -pedantic -I/home/hexu/local/google_benchmark/include -Wl,-rpath,/home/hexu/local/google_benchmark/lib -Wl,--enable-new-dtags -L/home/hexu/local/google_benchmark/lib -pthread -lbenchmark
$ ./benchmark 
```

我们就会得到如下输出：

![benchmark google](benchmark_google.png)

通过结果我们可以知道在特定CPU配置和系统负载情况下的时延：51099 ns（由于是单线程模式下，墙上时钟和CPU时钟结果一样）。

有了基准测试数据，以及测试性能的统一方式，我们接下来就可以开始实现我们的内存池了。

**内存池版本1：专用 Rational 内存管理器**

为避免频繁地使用默认内存管理器，Rational 类要维护一个预先分配的 Rational 对象的静态连接列表，该列表列出空闲的可用对象。
当需要 Rational 对象时，可以从空闲列表中取出一个，使用后再把它放回空闲列表以便今后分配。

我们声明了一个辅助结构来连接空闲列表的相邻元素。

```cpp
class NextOnFreeList {
public:
    NextOnFreeList* next;
};
```

空闲列表被声明为一个由NextOnFreeList元素组成的列表。

```cpp
class Rational {
public:
    // ...

private:
	static NextOnFreeList* freeList;

private:
    // ...
};
```

虽然空闲列表的每个元素都声明为 NextOnFreeList 结构，但是它们
仍然是 Rational 对象。创建一个元素时，我们给它分配足够大的空间以包含
Rational 对象。为了遍历对象，每个 Rational 对象的前几个字节均用于
指向空闲列表的下一个对象。我们可以通过把 Rational 对象转换成指向
NextOnFreeList 类型的指针来实现。

这样，该空闲列表有双重身份：其一是 Rational 对象的序列，其二是 NextOnFreeList 元素的序列。

![figure-6.1](figure-6.1.png)

将空闲列表声明为 Rational 类的静态成员， Rational 的操作符 new() 和 delete() 可以管理该静态列表。
这两个操作符重载了对应的全局操作符。

```cpp
class Rational {
public:
    enum { EXPANSION_SIZE = 32 };

	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void* operator new(size_t size); 
	void operator delete(void* doomed, size_t size);

	static void newMemPool() { expandTheFreeList(); }
	static void deleteMemPool();

	static void expandTheFreeList();

private:
	static NextOnFreeList* freeList;

private:
	int n;	// Numerator
	int d;	// Denominator
};
```

new() 从空闲列表头部分配一个 Rational 对象所需的内存。如果列表为空，就扩展列表。

```cpp
void* Rational::operator new(size_t size) {
    if (nullptr == freeList) {
        expandTheFreeList();
    }

    NextOnFreeList* head = freeList;
    freeList = head->next;

    return head;
}
```

delete() 把 Rational 对象的内存直接添加到空闲列表的头部，以释放一个 Rational 对象。

```cpp
void Rational::operator delete(void* doomed, size_t size) {
    NextOnFreeList* head = static_cast<NextOnFreeList*>(doomed);

    head->next = freeList;
    freeList = head;
}
```

当空闲列表用完时，需要从堆上分配更多的 Rational 对象内存。

```cpp
void Rational::expandTheFreeList() {
	size_t size = (sizeof(Rational) > sizeof(NextOnFreeList*)) ?
		sizeof(Rational) : sizeof(NextOnFreeList*);

	NextOnFreeList* runner = reinterpret_cast<NextOnFreeList*>(new char[size]);

	freeList = runner;
	for (int i = 0; i < EXPANSION_SIZE; i++) {
		runner->next =
			reinterpret_cast<NextOnFreeList*>(new char[size]);
		runner = runner->next;
	}
	runner->next = nullptr;
}
```

完整的工程链接在：[recipe-01](https://github.com/hexu1985/Collection.Of.Cpp.Utility.Tools/tree/master/code/memory_pool/recipe-01)

编译并运行代码：

```shell
$ clang++ -o example example.cpp rational.cpp -g -O3 -mavx2 -Wall -pedantic
$ ./example 
use time: 168 ms
```

直观的看到，性能的确有一定的提升，我们再使用Google Benchmark库测试的程序跑一遍：

```shell
$ clang++ -o benchmark benchmark.cpp rational.cpp -g -O3 -mavx2 -Wall -pedantic -I/home/hexu/local/google_benchmark/include -Wl,-rpath,/home/hexu/local/google_benchmark/lib -Wl,--enable-new-dtags -L/home/hexu/local/google_benchmark/lib -pthread -lbenchmark
$ ./benchmark
```

我们就会得到如下输出：

![recipe-01 google](recipe-01_google.png)

17534 ns这个结果和基于chrono::steady_clock的计时统计是一致的。

然后我们再来看看热点分布。



