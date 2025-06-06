# Fast DDS HistoryCache 详解

HistoryCache 是 Fast DDS (Fast RTPS) 中一个核心的缓存管理机制，负责存储和管理数据样本(CacheChange)的历史记录。它在数据发布(WriterHistory)和订阅(ReaderHistory)两端都扮演着关键角色。

## 1. HistoryCache 基本概念

### 1.1 主要功能
- 存储已发送/接收的数据样本(CacheChange_t)
- 维护样本的序列号和状态
- 实现数据的历史管理策略
- 支持可靠性传输的重传机制

### 1.2 关键组件
```cpp
class HistoryCache {
    std::vector<CacheChange_t*> changes_;  // 存储CacheChange指针的容器
    SequenceNumber_t next_sequence_;       // 下一个序列号
    // 其他成员变量...
};
```

## 2. CacheChange_t 结构

CacheChange_t 是HistoryCache管理的基本单元：

```cpp
struct CacheChange_t {
    ChangeKind_t kind;              // 数据类型(ALIVE, NOT_ALIVE)
    SequenceNumber_t sequenceNumber;// 序列号
    InstanceHandle_t instanceHandle;// 实例句柄
    SerializedPayload_t serializedPayload; // 实际数据负载
    
    // 状态相关
    bool isRead;                   // 是否已被读取
    int32_t writerGUID;            // 写入者标识
    // 其他成员...
};
```

## 3. HistoryCache 工作流程

### 3.1 写入端(WriterHistory)流程

1. **数据写入**：
```cpp
DataWriter::write() → WriterHistory::add_change() → HistoryCache::add_change()
```

2. **添加新样本**：
   - 分配新的CacheChange_t
   - 设置序列号(自动递增)
   - 将数据拷贝到serializedPayload
   - 添加到changes_容器

3. **历史管理**：
   - 检查ResourceLimitsQos限制
   - 根据HistoryQosPolicy决定是否丢弃旧样本

### 3.2 读取端(ReaderHistory)流程

1. **数据接收**：
```cpp
MessageReceiver → RTPSReader → ReaderHistory::add_change() → HistoryCache::add_change()
```

2. **样本处理**：
   - 验证序列号连续性
   - 检查重复数据
   - 更新样本状态(ALIVE/NOT_ALIVE)

## 4. 缓存管理策略

### 4.1 HistoryQosPolicy 策略

| 策略类型 | 行为描述 | 适用场景 |
|---------|---------|---------|
| KEEP_LAST | 只保留最新的N个样本 | 高频数据流 |
| KEEP_ALL | 保留所有样本直到被确认 | 关键数据 |

### 4.2 ResourceLimitsQos 限制

```xml
<resource_limits>
    <max_samples>1000</max_samples>       <!-- 最大样本数 -->
    <max_instances>10</max_instances>     <!-- 最大实例数 -->
    <max_samples_per_instance>100</max_samples_per_instance> <!-- 每个实例最大样本数 -->
</resource_limits>
```

### 4.3 样本清理机制

1. **自动清理**：
   - 基于HistoryQosPolicy策略
   - 基于ACKNACK确认(可靠传输)
   - 基于LifespanQosPolicy过期

2. **手动清理**：
```cpp
writer->clear_history();  // 清除Writer历史
reader->clear_history();  // 清除Reader历史
```

## 5. 关键操作API

### 5.1 添加变更
```cpp
bool add_change(CacheChange_t* change);
```

### 5.2 移除变更
```cpp
bool remove_change(CacheChange_t* change);
```

### 5.3 查找变更
```cpp
CacheChange_t* find_change(SequenceNumber_t seq);
```

### 5.4 获取最小/最大序列号
```cpp
SequenceNumber_t get_min_seq_num();
SequenceNumber_t get_max_seq_num();
```

## 6. 性能优化考虑

1. **预分配策略**：
   - 提前分配足够大的changes_容器
   - 使用对象池管理CacheChange_t

2. **内存管理**：
   - 使用PayloadPool共享大数据负载
   - 实现零拷贝(zero-copy)访问

3. **并发控制**：
   - 读写锁保护changes_容器
   - 原子操作管理序列号

## 7. 典型问题排查

1. **历史缓存满**：
   - 症状：数据写入失败
   - 解决：调整HistoryQosPolicy.depth或ResourceLimits

2. **序列号不连续**：
   - 症状：订阅端接收数据有间隔
   - 解决：检查网络丢包或发送端配置

3. **内存泄漏**：
   - 症状：内存持续增长
   - 解决：确保正确调用remove_change/release_change

HistoryCache是Fast DDS实现高效数据分发的核心组件，合理配置其参数对系统性能有重大影响。建议根据实际应用场景(数据频率、可靠性要求等)仔细调优相关QoS策略。
