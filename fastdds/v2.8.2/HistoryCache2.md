当 Fast DDS 中的 **HistoryCache** 达到容量上限时，其行为会根据配置的 QoS 策略和资源限制而有所不同，以下是详细分析：

---

### **1. 触发条件**
HistoryCache 满的两种情况：
1. **样本数达到 `ResourceLimitsQos.max_samples`**  
2. **历史深度超过 `HistoryQosPolicy.depth`**（仅 `KEEP_LAST` 策略）

---

### **2. 不同场景下的行为**

#### **场景 1：发送端（WriterHistory）缓存满**
| QoS 配置                | 行为                                                                 |
|-------------------------|----------------------------------------------------------------------|
| **Reliable + KEEP_LAST** | 新数据覆盖最旧的未确认样本（循环缓冲），可能导致订阅端丢失数据。       |
| **Reliable + KEEP_ALL**  | `write()` 阻塞或返回 `false`，直到有样本被确认释放（通过 ACKNACK）。   |
| **Best-Effort**         | 直接丢弃新数据（无阻塞），无论是否可靠。                              |

**代码逻辑示例**：
```cpp
bool WriterHistory::add_change(CacheChange_t* change) {
    if (isFull()) {
        if (qos_.history.kind == KEEP_LAST) {
            remove_oldest_unconfirmed(); // 覆盖最旧样本
        } else {
            return false; // 写入失败
        }
    }
    // ...添加新样本
}
```

#### **场景 2：接收端（ReaderHistory）缓存满**
| QoS 配置                | 行为                                                                 |
|-------------------------|----------------------------------------------------------------------|
| **Reliable**            | 发送 NACK 拒绝新数据，要求发送端重传丢失样本（流控机制）。             |
| **Best-Effort**         | 直接丢弃新数据，无反馈。                                             |

---

### **3. 关键影响因素**
#### **(1) HistoryQosPolicy.kind**
- **`KEEP_LAST`**：保留最新的 N 个样本（`depth` 决定 N），超限时覆盖旧数据。  
  ```xml
  <history>
      <kind>KEEP_LAST</kind>
      <depth>50</depth>  <!-- 保留最新50条 -->
  </history>
  ```
- **`KEEP_ALL`**：保留所有样本直到被确认，超限时阻塞或失败。

#### **(2) ReliabilityQosPolicy**
- **`RELIABLE`**：缓存满可能导致阻塞（等待确认）或覆盖（`KEEP_LAST`）。  
- **`BEST_EFFORT`**：直接丢弃数据，无保障。

#### **(3) ResourceLimitsQos**
限制缓存的最大样本数和实例数：
```xml
<resource_limits>
    <max_samples>1000</max_samples>      <!-- 总样本数上限 -->
    <max_instances>5</max_instances>     <!-- 最大实例数 -->
</resource_limits>
```

---

### **4. 问题排查与解决方案**

#### **现象：数据丢失或写入阻塞**
- **检查方向**：
  1. 监控 HistoryCache 使用量（通过 Fast DDS 统计模块或日志）。
  2. 确认 QoS 配置是否匹配场景需求。

- **解决方案**：
  ```xml
  <!-- 调整 QoS 示例 -->
  <history>
      <kind>KEEP_LAST</kind>
      <depth>100</depth> <!-- 增大深度 -->
  </history>
  <resource_limits>
      <max_samples>5000</max_samples> <!-- 扩大缓存 -->
  </resource_limits>
  ```

#### **现象：订阅端收不到数据**
- **检查方向**：
  1. 确认订阅端的 `ReaderHistory` 是否也满（可能因未及时 `take()` 数据）。
  2. 检查网络是否丢包（特别是 Best-Effort 模式）。

- **解决方案**：
  ```cpp
  // 订阅端及时读取数据
  if (reader->take_next_sample(&data, &info) == ReturnCode_t::RETCODE_OK) {
      // 处理数据
  }
  ```

---

### **5. 设计建议**
1. **高频数据场景**：  
   - 使用 `KEEP_LAST` + 合理 `depth`，避免内存爆炸。
   - 启用 `FlowController` 限制发送速率。

2. **关键数据场景**：  
   - 使用 `RELIABLE` + `KEEP_ALL`，确保数据不丢失。
   - 增大 `max_samples` 或优化应用层处理速度。

3. **实时监控**：  
   - 通过 `RTPSWriter::get_unacked_count()` 监控未确认样本数。
   - 使用 Fast DDS 的 `monitor-service` 模块实时查看缓存状态。

---

### **总结**
HistoryCache 满时的行为本质是 **可靠性（Reliability）** 和 **实时性（Latency）** 的权衡：  
- 若需绝对可靠（如金融交易），配置 `RELIABLE` + `KEEP_ALL` + 大缓存。  
- 若需低延迟（如传感器数据），配置 `BEST_EFFORT` + `KEEP_LAST` + 适当深度。  
通过合理配置 QoS 和及时监控，可以避免缓存满导致的意外行为。
