# Fast DDS 中的 StackAllocatedSequence 类型说明

`StackAllocatedSequence` 是 Fast DDS (Fast RTPS) 中的一个模板类，用于高效地管理序列数据。它是 Fast DDS 类型系统的一部分，主要用于 IDL 生成的类型。

## 基本概念

`StackAllocatedSequence` 是一个在栈上分配内存的序列容器，具有以下特点：

1. **栈分配**：与动态分配的序列不同，它在栈上预分配固定大小的内存
2. **固定容量**：创建时就确定了最大容量
3. **高效性**：避免了动态内存分配的开销
4. **轻量级**：适合小型数据序列

## 典型用法

```cpp
#include <fastrtps/utils/StackAllocatedSequence.h>

// 定义一个最大容量为10的int序列
eprosima::fastrtps::StackAllocatedSequence<int, 10> mySequence;

// 添加元素
mySequence.push_back(1);
mySequence.push_back(2);

// 访问元素
int first = mySequence[0];
```

## 主要成员函数

- `push_back(const T& item)`: 添加元素到序列末尾
- `size()`: 返回当前元素数量
- `operator[](size_t index)`: 通过索引访问元素
- `clear()`: 清空序列
- `capacity()`: 返回最大容量

## 使用场景

`StackAllocatedSequence` 特别适合以下情况：
- 已知序列最大大小的场景
- 需要避免动态内存分配的性能关键代码
- 小型数据序列处理
- 实时系统或嵌入式环境

## 注意事项

1. 如果尝试添加超过预分配容量的元素，会导致未定义行为(通常崩溃)
2. 与动态分配的序列相比，灵活性较低
3. 适合已知上限的小型数据集

Fast DDS 使用这种类型来优化性能，特别是在类型系统内部和IDL生成的代码中。