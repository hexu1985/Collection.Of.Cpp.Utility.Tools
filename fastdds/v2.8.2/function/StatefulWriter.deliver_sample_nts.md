# **`StatefulWriter::deliver_sample_nts` 函数说明**

`StatefulWriter::deliver_sample_nts` 是 Fast DDS（FastRTPS）中 `StatefulWriter` 类的关键内部函数，用于 **非线程安全（NTS = Non-Thread-Safe）** 地向所有匹配的 `Reader` 投递数据样本。该函数通常由更高层的线程安全方法（如 `add_change()`）调用。

---

## **1. 函数功能**
- **核心作用**：将 `CacheChange`（数据变更）传递给所有匹配的 `ReaderProxy`。
- **主要任务**：
  - 遍历所有匹配的 `ReaderProxy`。
  - 将数据变更添加到每个 `ReaderProxy` 的待发送队列。
  - 触发数据发送（可能立即发送或等待批处理）。
- **适用场景**：
  - 新数据发布时调用。
  - 数据重传时调用（如收到 `NACK` 时）。

---

## **2. 函数签名**
```cpp
void StatefulWriter::deliver_sample_nts(
    CacheChange_t* change,
    const std::vector<GUID_t>& remote_readers,
    bool inline_qos);
```
### **参数说明**
| 参数 | 类型 | 说明 |
|------|------|------|
| `change` | `CacheChange_t*` | 要投递的数据变更（包含实际数据） |
| `remote_readers` | `std::vector<GUID_t>` | 目标 `Reader` 的 GUID 列表（为空表示所有匹配的 Reader） |
| `inline_qos` | `bool` | 是否在消息中内联携带 QoS 参数 |

---

## **3. 内部逻辑**
### **（1）数据投递流程**
1. **检查 `remote_readers`**：
   - 如果为空，则遍历 `matched_readers_`（所有匹配的 `ReaderProxy`）。
   - 如果指定了 `remote_readers`，则只投递给列表中的 `Reader`。

2. **遍历 `ReaderProxy`**：
   - 对每个目标 `ReaderProxy`，调用 `add_change()` 将变更加入其待发送队列。
   - 如果 `inline_qos` 为 `true`，会在消息中嵌入 QoS 参数。

3. **触发发送**：
   - 可能直接调用 `send_unsent_changes()` 发送数据。
   - 也可能等待批处理（如 `RTPSMessageGroup` 机制）。

### **（2）关键调用关系**
- 被 `add_change()` 调用（入口点）。
- 内部调用 `ReaderProxy::add_change()` 管理每个 `Reader` 的待发送队列。

---

## **4. 代码示例（伪代码）**
```cpp
void StatefulWriter::deliver_sample_nts(
    CacheChange_t* change,
    const std::vector<GUID_t>& remote_readers,
    bool inline_qos)
{
    // 1. 确定目标 Reader 列表
    auto readers = remote_readers.empty() ? matched_readers_ : filter_matched_readers(remote_readers);

    // 2. 遍历所有目标 ReaderProxy
    for (auto& reader_proxy : readers)
    {
        // 3. 将变更添加到 ReaderProxy 的待发送队列
        reader_proxy->add_change(change, inline_qos);
    }

    // 4. 可能触发立即发送（取决于配置）
    if (is_asynchronous_ == false)
    {
        send_unsent_changes();
    }
}
```

---

## **5. 关键注意事项**
### **（1）非线程安全（NTS）**
- 该函数 **不包含锁机制**，调用者必须确保线程安全（通常由 `add_change()` 加锁调用）。

### **（2）可靠性处理**
- 如果 `Reader` 启用了 `RELIABLE` QoS，`deliver_sample_nts` 会确保数据进入重传队列，直到收到 `ACK`。

### **（3）性能优化**
- 可能使用批处理（如 `RTPSMessageGroup`）减少网络包数量。
- `inline_qos` 可减少额外 QoS 消息的开销。

---

## **6. 与相关函数的交互**
| 函数 | 关系 |
|------|------|
| `add_change()` | 调用 `deliver_sample_nts`（加锁后） |
| `send_unsent_changes()` | 可能被 `deliver_sample_nts` 触发 |
| `ReaderProxy::add_change()` | 实际管理每个 `Reader` 的数据队列 |

---

## **7. 典型调用场景**
1. **新数据发布**：
   ```cpp
   writer->add_change(change); // 内部调用 deliver_sample_nts
   ```
2. **数据重传**：
   ```cpp
   // 收到 NACK 后，重传特定序列号的数据
   deliver_sample_nts(change, {reader_guid}, false);
   ```

---

## **8. 总结**
- **角色**：`StatefulWriter` 数据分发的核心底层函数。
- **关键行为**：非线程安全地向 `ReaderProxy` 投递数据。
- **调用约束**：必须由线程安全的上层方法（如 `add_change()`）调用。
- **优化点**：支持批处理和内联 QoS 以减少开销。

该函数是 Fast DDS 可靠数据传输的关键环节，理解其逻辑有助于调试数据分发问题或优化性能。