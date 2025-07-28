在 **Fast DDS（Fast RTPS）** 中，`Liveliness QoS` 和 `discovery_config.leaseDuration` 都与 **存活检测** 相关，但它们的应用场景和作用对象不同。以下是两者的关系详解：

---

### **1. 核心区别**
| **特性**               | **Liveliness QoS**                          | **discovery_config.leaseDuration**          |
|------------------------|--------------------------------------------|--------------------------------------------|
| **作用对象**           | 控制 **DataWriter** 和 **DataReader** 之间的存活检测。 | 控制 **DomainParticipant** 之间的存活检测（PDP/SPDP 协议）。 |
| **协议层级**           | 应用层（Endpoint 级别）。                   | 发现层（Participant 级别）。                |
| **默认值**             | `lease_duration = 无限`（需显式设置）。     | 通常默认 `leaseDuration = 30秒`（可配置）。 |
| **配置位置**           | DataWriter/DataReader 的 QoS。              | Participant 的 `builtin.discovery_config`。 |

---

### **2. 功能对比**
#### **(1) `Liveliness QoS`**
- **用途**：  
  确保订阅者（`DataReader`）能检测到发布者（`DataWriter`）是否存活。  
  - 如果 `DataWriter` 在 `lease_duration` 内未发送数据或心跳，`DataReader` 会触发 `on_liveliness_changed` 回调。  
  - 影响数据流的可靠性（例如，失活的 `DataWriter` 可能触发数据清理）。

- **配置示例**：  
  ```cpp
  DataWriterQos writer_qos;
  writer_qos.liveliness().kind = AUTOMATIC_LIVELINESS_QOS;
  writer_qos.liveliness().lease_duration = Duration_t(5, 0);  // 5秒超时
  writer_qos.liveliness().announcement_period = Duration_t(1, 0);  // 1秒心跳
  ```

#### **(2) `discovery_config.leaseDuration`**
- **用途**：  
  控制 **Participant 发现协议（PDP/SPDP）** 的存活检测。  
  - 如果某个 `DomainParticipant` 在 `leaseDuration` 内未发送声明（Announcement），其他 Participant 会认为它已离线。  
  - 影响全局节点发现（例如，失活的 Participant 会从发现列表中移除）。

- **配置示例**：  
  ```cpp
  RTPSParticipantAttributes params;
  params.builtin.discovery_config.leaseDuration = Duration_t(30, 0);  // 30秒超时
  params.builtin.discovery_config.leaseAnnouncement = Duration_t(3, 0);  // 3秒心跳
  ```

---

### **3. 协同工作场景**
- **场景 1**：  
  - `Participant A` 的 `leaseDuration` 超时 → 其他 Participant 认为 `A` 已离线，移除其所有 `DataWriters/Readers`。  
  - `Participant A` 的 `DataWriter` 的 `Liveliness QoS` 也会因 Participant 离线而失效。

- **场景 2**：  
  - `DataWriter` 的 `Liveliness` 超时 → 仅影响该 `DataWriter` 关联的 `DataReaders`，不影响其他 Participant 的发现状态。

---

### **4. 关键注意事项**
1. **优先级**：  
   - 如果 `Participant` 因 `leaseDuration` 超时被移除，其下所有 `DataWriters` 的 `Liveliness` 检测将自动失效。

2. **时间参数协调**：  
   - 推荐 `leaseDuration`（Participant 级） **≥** `DataWriter` 的 `lease_duration`（Endpoint 级），避免矛盾状态。  
   - 例如：  
     ```plaintext
     discovery_config.leaseDuration = 30秒  
     DataWriter.liveliness.lease_duration = 10秒  
     ```  
     这样，`DataWriter` 的失活会先被检测到，而 `Participant` 的离线检测作为最终保障。

3. **网络影响**：  
   - 缩短 `leaseDuration` 或 `lease_duration` 会提高故障检测速度，但增加网络负载（更多心跳包）。

---

### **5. 配置建议**
#### **(1) 高可靠性场景**
```xml
<!-- Participant 级配置 -->
<discovery_config>
    <leaseDuration>
        <sec>60</sec>  <!-- 较长超时，容忍临时网络波动 -->
    </leaseDuration>
    <leaseAnnouncement>
        <sec>5</sec>   <!-- 适度心跳间隔 -->
    </leaseAnnouncement>
</discovery_config>

<!-- DataWriter 级配置 -->
<liveliness>
    <kind>AUTOMATIC</kind>
    <lease_duration>
        <sec>10</sec>  <!-- 快速检测 Writer 失活 -->
    </lease_duration>
    <announcement_period>
        <sec>2</sec>   <!-- 高频心跳 -->
    </announcement_period>
</liveliness>
```

#### **(2) 低带宽环境**
```cpp
// 减少心跳频率
params.builtin.discovery_config.leaseDuration = Duration_t(120, 0);  // 120秒超时
params.builtin.discovery_config.leaseAnnouncement = Duration_t(10, 0);  // 10秒心跳

writer_qos.liveliness().lease_duration = Duration_t(30, 0);  // 30秒超时
writer_qos.liveliness().announcement_period = Duration_t(5, 0);  // 5秒心跳
```

---

### **6. 总结**
- **`Liveliness QoS`**：细粒度控制 **DataWriter/Reader** 的存活检测，影响数据流。  
- **`discovery_config.leaseDuration`**：粗粒度控制 **Participant** 的存活检测，影响节点发现。  
- **协作关系**：  
  - Participant 离线会强制终止其下所有 Endpoint 的活跃状态。  
  - 合理配置两者的时间参数可平衡 **故障检测速度** 和 **网络开销**。
