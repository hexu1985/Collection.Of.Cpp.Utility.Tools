在 Fast DDS 中，**Payload Pool** 如果没有可用资源（即无法分配新的 Payload 内存），系统的行为会根据配置和场景的不同而有所差异，通常会导致以下后果：

---

### **1. 直接表现**
- **`get_payload()` 返回失败**  
  当应用程序调用 `DataWriter::write()` 时，底层会尝试从 Payload Pool 分配内存。如果 Pool 耗尽：
  ```cpp
  CacheChange_t* change = writer->new_change();
  if (!payload_pool_->get_payload(data.size(), *change)) {
      // 分配失败，返回错误或抛出异常
  }
  ```
  - **默认行为**：Fast DDS 会返回 `false`，导致数据无法发送。
  - **用户需处理**：应用程序需要捕获此错误（如重试、丢弃数据或扩容 Pool）。

---

### **2. 对数据传输的影响**
#### **(1) 实时性场景（如 Best-Effort）**
- **数据丢失**：若未处理分配失败，当前数据会被丢弃，但后续数据可能继续发送（取决于历史缓存配置）。
- **无阻塞**：Best-Effort 模式下，不会等待资源释放。

#### **(2) 可靠性场景（如 Reliable）**
- **发送阻塞**：  
  可靠传输（RELIABLE QoS）下，`RTPSWriter` 可能因无法缓存新数据而阻塞，直到：
  - 订阅端确认部分数据（释放已确认的 Payload）。
  - Payload Pool 超时或手动扩容。

---

### **3. 资源耗尽的常见原因**
| 原因                | 解决方案                                                                 |
|---------------------|--------------------------------------------------------------------------|
| **Pool 大小不足**   | 增大 `payload_pool` 的预分配大小（如 XML 中 `<size>20MB</size>`）。       |
| **内存泄漏**        | 检查是否漏调 `release_payload()`（如未释放已确认的 `CacheChange_t`）。    |
| **高频率大数据**    | 优化数据分片或降低发送频率，使用 `LoanSample` 减少拷贝。                  |
| **订阅端处理慢**    | 调整 `FlowController` 或增加 `HistoryQosPolicy.depth` 缓冲数据。          |

---

### **4. 相关 QoS 和配置**
#### **(1) 动态扩容（部分实现支持）**
- 某些 Payload Pool 实现（如 `BufferPool`）支持动态扩展，但可能引入性能抖动。
  ```cpp
  BufferPool pool(initial_size, max_size); // 允许从 initial_size 扩展到 max_size
  ```

#### **(2) 资源限制策略**
- 通过 `ResourceLimitsQosPolicy` 限制最大样本数，间接控制 Payload 使用量：
  ```xml
  <resource_limits>
      <max_samples>1000</max_samples> <!-- 限制样本数 -->
  </resource_limits>
  ```

---

### **5. 错误处理建议**
#### **(1) 监控与告警**
```cpp
if (!payload_pool_->get_payload(size, change)) {
    LOG_ERROR("Payload Pool exhausted!"); // 触发告警
    return false;
}
```

#### **(2) 自动恢复机制**
- **重试策略**：延迟后重试（适用于短暂峰值）。
- **降级处理**：临时切换为 Best-Effort 模式或丢弃低优先级数据。

#### **(3) 预防措施**
- **预热 Pool**：启动时预分配足够资源。
  ```cpp
  std::vector<CacheChange_t> preallocated_changes;
  for (int i = 0; i < 100; ++i) {
      auto change = writer->new_change();
      payload_pool_->get_payload(default_size, *change);
      preallocated_changes.push_back(change);
  }
  ```

---

### **总结**
当 Fast DDS 的 Payload Pool 耗尽时，**默认行为是数据发送失败**，但具体表现取决于 QoS 配置和错误处理逻辑。关键应对措施包括：
1. **合理配置 Pool 大小**（静态预分配或动态扩展）。
2. **严格管理 Payload 生命周期**（及时释放）。
3. **设计容错机制**（如降级、重试）。

对于高性能场景（如自动驾驶、工业控制），建议通过压力测试确定 Pool 的合理容量，并结合监控工具（如 Fast DDS 的统计模块）实时跟踪资源使用情况。
