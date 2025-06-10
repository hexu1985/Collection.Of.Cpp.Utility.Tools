# Fast DDS 中的 CacheChange_t 类型说明

`CacheChange_t` 是 Fast DDS (前身为 Fast RTPS) 中的一个核心数据结构，用于表示数据分发服务(DDS)中的缓存变更。它在发布/订阅模型中扮演重要角色，用于存储和管理数据的变更历史。

## 基本结构

`CacheChange_t` 定义在 Fast DDS 的源代码中，主要包含以下重要字段：

```cpp
struct CacheChange_t
{
    // 变更的唯一标识符
    ChangeKind_t kind;          // 变更类型(ALIVE, NOT_ALIVE, NOT_ALIVE_DISPOSED等)
    SequenceNumber_t sequenceNumber; // 序列号
    GUID_t writerGUID;          // 写入此变更的写入者GUID
    
    // 数据相关字段
    uint32_t serializedPayload_length; // 有效载荷长度
    SerializedPayload_t serializedPayload; // 序列化后的数据
    
    // 时间戳
    Time_t sourceTimestamp;
    Time_t receptionTimestamp;
    
    // 其他元数据
    InstanceHandle_t instanceHandle; // 关联的实例句柄
    bool isRead;                    // 是否已被读取
    
    // 内存管理相关
    MemoryManagementPolicy_t memoryPolicy; // 内存管理策略
    void* payload_owner;                  // 有效载荷所有者
};
```

## 主要成员说明

1. **kind**: 表示变更的类型，常见值包括：
   - `ALIVE`: 活跃的数据变更
   - `NOT_ALIVE`: 数据不再活跃
   - `NOT_ALIVE_DISPOSED`: 数据被处置

2. **sequenceNumber**: 序列号，用于确保数据的有序性

3. **writerGUID**: 标识写入此变更的写入者

4. **serializedPayload**: 包含实际的数据负载，使用 `SerializedPayload_t` 类型表示

5. **timestamps**: 包含源时间戳和接收时间戳

6. **instanceHandle**: 关联的数据实例句柄

## 内存管理

`CacheChange_t` 支持不同的内存管理策略，通过 `memoryPolicy` 指定：
- `PREALLOCATED_MEMORY_MODE`: 预分配内存
- `PREALLOCATED_WITH_REALLOC_MEMORY_MODE`: 预分配但可重新分配
- `DYNAMIC_RESERVE_MEMORY_MODE`: 动态保留内存
- `DYNAMIC_REUSABLE_MEMORY_MODE`: 动态可重用内存

## 使用场景

`CacheChange_t` 主要用于：
1. 发布者缓存待发送的数据变更
2. 订阅者缓存接收到的数据变更
3. 历史缓存管理
4. 可靠传输机制中的重传管理

## 注意事项

- 使用后需要正确释放资源，特别是 `serializedPayload`
- 序列号管理对于数据一致性至关重要
- 内存管理策略的选择会影响性能

这个结构体是 Fast DDS 实现 DDS 规范中历史缓存和可靠传输机制的基础组件。