# CDRMessage_t 类型说明 (Fast DDS)

`CDRMessage_t` 是 Fast DDS (前身为 Fast RTPS) 中的一个核心数据结构，用于处理 CDR (Common Data Representation) 格式的消息。CDR 是 OMG (Object Management Group) 定义的一种标准化数据序列化格式，广泛用于 DDS (Data Distribution Service) 实现中。

## 基本结构

在 Fast DDS 中，`CDRMessage_t` 通常定义如下（具体实现可能略有不同）：

```c
typedef struct CDRMessage_t
{
    // 消息缓冲区
    octet* buffer;
    
    // 缓冲区最大容量
    uint32_t max_size;
    
    // 当前消息长度
    uint32_t length;
    
    // 当前读写位置
    uint32_t pos;
    
    // 字节序标志 (大端或小端)
    bool msg_endian;
} CDRMessage_t;
```

## 主要字段说明

1. **buffer**: 
   - 指向实际存储 CDR 格式数据的字节数组
   - 使用 `octet` 类型（通常是无符号8位整数）

2. **max_size**:
   - 缓冲区最大容量（以字节为单位）
   - 在初始化时设置

3. **length**:
   - 当前消息的实际长度（以字节为单位）
   - 随着数据写入/读取而变化

4. **pos**:
   - 当前读写位置（偏移量）
   - 用于跟踪序列化/反序列化的进度

5. **msg_endian**:
   - 消息的字节序（大端或小端）
   - Fast DDS 需要处理不同端序系统间的通信

## 主要功能

`CDRMessage_t` 用于：

1. **序列化**：将数据结构转换为 CDR 格式的字节流
2. **反序列化**：从 CDR 格式字节流重建数据结构
3. **消息处理**：在 Fast DDS 内部传输和存储数据

## 常用操作函数

Fast DDS 提供了一系列函数来操作 `CDRMessage_t`，例如：

- `CDRMessage_init()`: 初始化消息结构
- `CDRMessage_reset()`: 重置消息（不清除缓冲区）
- `CDRMessage_read*()`: 读取各种数据类型（如 readUInt32, readInt16 等）
- `CDRMessage_write*()`: 写入各种数据类型
- `CDRMessage_serialize()`: 序列化数据
- `CDRMessage_deserialize()`: 反序列化数据

## 使用示例

```c
// 创建并初始化消息
CDRMessage_t message;
char buffer[1000];
CDRMessage_init(&message, 1000, (octet*)buffer);

// 写入数据
uint32_t value = 42;
CDRMessage_writeUInt32(&message, value);

// 读取数据
uint32_t readValue;
CDRMessage_readUInt32(&message, &readValue);
```

## 注意事项

1. 缓冲区管理：需要确保缓冲区足够大以避免溢出
2. 字节序处理：在异构系统中需要正确处理字节序转换
3. 线程安全：`CDRMessage_t` 本身不是线程安全的，多线程访问需要额外同步

`CDRMessage_t` 是 Fast DDS 实现数据序列化和网络传输的基础，理解它的工作原理对于深入使用或扩展 Fast DDS 非常重要。
