# Fast DDS 中的 `WriterHistory` 类型说明

`WriterHistory` 是 Fast DDS 中用于管理数据写入端(DataWriter)历史记录的核心组件，负责维护已发布但尚未被确认接收的数据样本。

## 基本概念

`WriterHistory` 是 DataWriter 的历史记录管理器，主要功能包括：
- 存储已发布的数据样本
- 管理样本的生命周期
- 实现历史QoS策略
- 支持可靠传输的重传机制

## 主要特性

### 1. 历史记录存储
- 维护已发送但未被确认的数据样本
- 支持多种历史QoS策略：
  - `KEEP_LAST_HISTORY_QOS` (保留最近N个样本)
  - `KEEP_ALL_HISTORY_QOS` (保留所有样本)

### 2. 样本管理
- 每个样本包含：
  - 序列化后的数据
  - 样本标识(SequenceNumber)
  - 状态信息(是否已确认)
- 自动淘汰过期样本

### 3. 与Writer的关系
- 每个DataWriter拥有一个独立的WriterHistory实例
- 负责将新样本传递给下层传输
- 处理来自Reader的确认消息(ACK/NACK)

## 关键数据结构

```cpp
class WriterHistory
{
public:
    // 添加新样本到历史记录
    bool add_payload(CacheChange_t* change);
    
    // 从历史记录中移除样本
    bool remove_change(CacheChange_t* change);
    
    // 获取历史记录中的样本
    HistoryIterator get_history_iterator() const;
    
    // QoS相关配置
    HistoryQosPolicy history_qos;
    ResourceLimitsQosPolicy resource_limits_qos;
    
private:
    // 实际存储样本的容器
    std::vector<CacheChange_t*> changes_;
    
    // 其他管理状态...
};
```

## 核心功能流程

### 数据发布流程
1. DataWriter创建新的CacheChange
2. 序列化应用数据到CacheChange
3. 调用WriterHistory::add_payload()
4. WriterHistory根据QoS策略管理样本
5. 传递给传输层发送

### 确认处理流程
1. 收到Reader的ACK/NACK
2. 更新对应样本的状态
3. 对于NACK，触发重传机制
4. 当样本被所有Reader确认后，可能被淘汰

## 配置参数

通过QoS策略配置：
```xml
<data_writer>
    <historyQos>
        <kind>KEEP_LAST</kind>
        <depth>50</depth>
    </historyQos>
    <resourceLimitsQos>
        <max_samples>1000</max_samples>
        <max_instances>10</max_instances>
        <max_samples_per_instance>100</max_samples_per_instance>
    </resourceLimitsQos>
</data_writer>
```

## 性能考量

1. **内存使用**：
   - 历史记录大小影响内存占用
   - 大历史深度需要更多内存

2. **传输效率**：
   - 保留历史样本支持重传
   - 但过多的历史样本会增加处理开销

3. **实时性**：
   - 历史清理操作可能引起延迟
   - 需要平衡实时性和可靠性

## 使用建议

1. 根据应用需求设置合适的history depth：
   - 实时系统：较小的depth
   - 可靠传输：足够大的depth

2. 监控history使用情况：
   - 避免因资源限制导致样本丢弃
   - 调整max_samples等参数

3. 对于关键数据：
   - 考虑使用KEEP_ALL策略
   - 确保重要样本不被过早淘汰

`WriterHistory` 是Fast DDS可靠传输的基础组件，合理配置对系统性能和可靠性有重要影响。