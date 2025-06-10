# **Fast DDS 中的 StatefulWriter 类型说明**

`StatefulWriter` 是 Fast DDS（FastRTPS）中实现 **可靠、有状态** 数据分发的核心类，继承自 `RTPSWriter`。它管理数据变更（`CacheChange`）的存储、发送、确认和重传，确保数据可靠地传输给匹配的 `StatefulReader`。

---

## **1. 基本概念**
`StatefulWriter` 是 **有状态写入器**，负责：
- 维护已发送数据的记录（历史缓存）
- 处理来自 `Reader` 的 **ACK/NACK** 反馈
- 管理数据重传（可靠性）
- 支持 **持久化**（Durability）机制（如 `TRANSIENT_LOCAL`）
- 处理 **心跳（Heartbeat）** 和 **GAP 消息** 的发送

---

## **2. 主要功能**
| 功能 | 说明 |
|------|------|
| **数据变更管理** | 存储待发送的 `CacheChange`，维护 `HistoryCache` |
| **可靠性支持** | 使用 **ACK/NACK** 机制确保数据可靠传输 |
| **心跳机制** | 定期发送 `Heartbeat` 通知 `Reader` 可用的数据 |
| **GAP 消息** | 通知 `Reader` 哪些数据可跳过（如已被删除） |
| **流控（Flow Control）** | 管理发送速率，避免网络拥塞 |
| **持久化支持** | 支持 `VOLATILE`、`TRANSIENT_LOCAL` 等持久化 QoS |

---

## **3. 关键成员变量**
| 变量 | 说明 |
|------|------|
| `ReaderProxyCollection matched_readers_` | 存储匹配的 `ReaderProxy`（每个订阅者对应一个代理） |
| `WriterHistory history_` | 存储待发送/已发送的 `CacheChange` |
| `SequenceNumber_t next_sequence_number_` | 下一个新数据的序列号 |
| `bool disable_positive_acks_` | 是否禁用 `ACK`（优化延迟） |
| `std::mutex mutex_` | 线程安全锁 |

---

## **4. 重要方法**
### **（1）数据发布相关**
| 方法 | 说明 |
|------|------|
| `new_change()` | 创建一个新的 `CacheChange` |
| `add_change()` | 将变更加入 `HistoryCache` |
| `remove_change()` | 从历史中移除变更 |

### **（2）ACK/NACK 处理**
| 方法 | 说明 |
|------|------|
| `process_acknack()` | 处理来自 `Reader` 的 `ACKNACK` 消息 |
| `process_heartbeat()` | 处理 `Reader` 的 `HEARTBEAT` 响应 |
| `send_heartbeat()` | 发送 `Heartbeat` 通知数据可用性 |

### **（3）数据重传**
| 方法 | 说明 |
|------|------|
| `send_any_unsent_changes()` | 发送所有未确认的数据 |
| `mark_fragment_as_sent()` | 标记分片数据已发送 |

### **（4）Reader 管理**
| 方法 | 说明 |
|------|------|
| `matched_reader_add()` | 添加新的 `ReaderProxy` |
| `matched_reader_remove()` | 移除不再匹配的 `Reader` |

---

## **5. 典型工作流程**
### **（1）数据发布**
```cpp
CacheChange_t* change = writer->new_change(...);
// 填充数据
writer->add_change(change);
```
1. `new_change()` 分配新的 `CacheChange`。
2. `add_change()` 将其加入 `HistoryCache` 并触发发送。

### **（2）ACK/NACK 处理**
- `Reader` 发送 `NACK` 请求缺失数据。
- `StatefulWriter` 检查 `ReaderProxy`，重传未确认的数据。

### **（3）心跳机制**
- `StatefulWriter` 定期发送 `Heartbeat`。
- `Reader` 回复 `ACKNACK`，指示需要哪些数据。

---

## **6. 与 StatelessWriter 的区别**
| 特性 | `StatefulWriter` | `StatelessWriter` |
|------|----------------|------------------|
| **可靠性** | ✅ 支持（ACK/NACK） | ❌ 仅 Best-Effort |
| **Reader 状态维护** | ✅ 每个 `Reader` 有 `ReaderProxy` | ❌ 无状态 |
| **持久化支持** | ✅ 支持 `TRANSIENT_LOCAL` | ❌ 仅 `VOLATILE` |
| **适用场景** | 可靠传输（如 `RELIABLE` QoS） | 低延迟，允许丢包 |

---

## **7. 在 Fast DDS 中的作用**
- **核心数据分发**：管理数据的可靠传输。
- **QoS 支持**：实现 `RELIABILITY`、`DURABILITY` 等 QoS。
- **流控机制**：避免网络拥塞。
- **与 `StatefulReader` 配合**：构成完整的可靠通信链路。

---

## **8. 示例代码**
```cpp
// 创建 StatefulWriter
StatefulWriter* writer = ...;

// 发布数据
CacheChange_t* change = writer->new_change(
    [](CacheChange_t* c) { /* 数据初始化 */ },
    ALIVE, /* 数据状态 */
    nullptr /* 数据拥有者 */
);
writer->add_change(change);

// 处理 ACKNACK（内部自动调用）
// 重传逻辑由 Fast DDS 自动管理
```

---

## **9. 总结**
`StatefulWriter` 是 Fast DDS 实现 **可靠数据传输** 的核心组件，适用于需要 **严格数据一致性** 的场景（如金融交易、自动驾驶等）。它通过 `ReaderProxy` 跟踪订阅者状态，并利用 **ACK/NACK + 心跳机制** 确保数据可靠送达。