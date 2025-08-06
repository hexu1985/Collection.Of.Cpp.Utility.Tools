# RTPSParticipantAllocationAttributes 类说明

`RTPSParticipantAllocationAttributes` 是 Fast DDS (Fast RTPS) 中的一个配置类，用于控制 RTPS 参与者(participant)的资源分配策略。这个类允许用户配置各种与内存分配相关的参数，以优化性能和资源使用。

## 主要功能

这个类主要用于配置 RTPS 参与者在运行时分配资源的策略，包括：

1. 预分配资源数量
2. 资源增长策略
3. 各种实体(如端点、读取器、写入器等)的最大数量限制

## 主要成员变量

以下是 `RTPSParticipantAllocationAttributes` 类中的主要配置参数：

```cpp
struct RTPSParticipantAllocationAttributes {
    // 远程服务器分配属性
    RemoteServerAllocationAttributes remote_servers;
    
    // 参与者分配限制
    uint32_t max_partitions;  // 最大分区数
    
    // 内置端点分配属性
    EntityAllocationAttributes builtin;
    
    // 用户定义的端点分配属性
    EntityAllocationAttributes user_data;
    
    // 读取器资源限制
    ResourceLimitedContainerConfig readers;
    
    // 写入器资源限制
    ResourceLimitedContainerConfig writers;
    
    // 发送缓冲区分配属性
    SendBuffersAllocationAttributes send_buffers;
    
    // 域参与者QoS分配属性
    DomainParticipantQosAllocationAttributes domain_participant_qos;
};
```

其中几个重要子结构：

### EntityAllocationAttributes
```cpp
struct EntityAllocationAttributes {
    uint32_t initial;  // 初始预分配数量
    uint32_t maximum;  // 最大允许数量
    uint32_t increment;  // 每次增长的增量
};
```

### ResourceLimitedContainerConfig
```cpp
struct ResourceLimitedContainerConfig {
    uint32_t initial;  // 初始预分配数量
    uint32_t maximum;  // 最大允许数量
    uint32_t increment;  // 每次增长的增量
    MemoryManagementPolicy_t memory_policy;  // 内存管理策略
};
```

## 使用示例

```cpp
#include <fastdds/rtps/attributes/RTPSParticipantAllocationAttributes.h>

// 创建分配属性配置
RTPSParticipantAllocationAttributes allocation;

// 配置内置端点
allocation.builtin.initial = 3;
allocation.builtin.maximum = 100;
allocation.builtin.increment = 1;

// 配置读取器
allocation.readers.initial = 5;
allocation.readers.maximum = 200;
allocation.readers.increment = 5;

// 配置写入器
allocation.writers.initial = 5;
allocation.writers.maximum = 200;
allocation.writers.increment = 5;

// 创建参与者时使用这些配置
RTPSParticipantAttributes participant_attr;
participant_attr.allocation = allocation;
```

## 内存管理策略

`memory_policy` 可以设置为以下值之一：
- `PREALLOCATED_MEMORY_MODE` - 预分配内存
- `PREALLOCATED_WITH_REALLOC_MEMORY_MODE` - 预分配但允许重新分配
- `DYNAMIC_RESERVE_MEMORY_MODE` - 动态保留内存
- `DYNAMIC_REUSABLE_MEMORY_MODE` - 动态可重用内存

## 应用场景

通过合理配置这些参数，可以在以下方面进行优化：
- 减少运行时内存分配次数
- 限制资源使用以避免内存耗尽
- 针对特定应用场景优化性能
- 在资源受限环境中控制内存占用

这个类特别适用于需要精确控制内存使用或需要高性能的场景，如嵌入式系统或实时系统。
