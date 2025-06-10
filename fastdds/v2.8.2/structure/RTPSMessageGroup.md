# Fast DDS 中的 RTPSMessageGroup 类型说明

RTPSMessageGroup 是 Fast DDS (FastRTPS) 中用于高效构造和发送 RTPS 消息的重要工具类，它提供了一种批量处理 RTPS 消息的机制。

## 基本概念

RTPSMessageGroup 是一个辅助类，用于：
- 将多个 RTPS 子消息(submessages)组合成单个消息
- 管理消息的序列化和发送
- 优化网络传输效率

## 主要功能

1. **消息批处理**：将多个子消息组合成单个网络数据包
2. **自动分片**：处理超过最大传输单元(MTU)限制的消息
3. **资源管理**：高效管理内存和网络资源
4. **序列化控制**：处理消息的序列化过程

## 关键成员和方法

### 重要成员变量
- `messages_`：存储待发送的消息列表
- `current_payload_`：当前正在构建的消息负载
- `max_payload_size_`：最大负载大小(基于MTU)

### 主要方法

1. **构造函数**：
   ```cpp
   RTPSMessageGroup(
       bool endianness,
       uint32_t max_payload_size,
       RTPSParticipantImpl* participant,
       LocatorSelectorSender* loc_selector,
       std::chrono::steady_clock::time_point max_blocking_time);
   ```

2. **添加子消息**：
   ```cpp
   void add_submessage(SubmessageHeader& subheader, const octet* data, uint32_t size);
   ```

3. **发送控制**：
   ```cpp
   void flush();  // 强制发送当前缓存的所有消息
   void reset();  // 重置消息组状态
   ```

4. **资源访问**：
   ```cpp
   CDRMessage_t& get_current_buffer();  // 获取当前缓冲区
   ```

## 使用模式

典型使用流程：
1. 创建 RTPSMessageGroup 对象
2. 添加多个子消息
3. 自动或手动触发消息发送
4. 对象销毁时自动清理资源

## 内部工作原理

1. 当添加子消息时，检查当前缓冲区剩余空间
2. 如果空间不足，自动发送当前消息并开始新消息
3. 维护消息的正确序列化和字节序
4. 处理与底层传输相关的细节

## 在 Fast DDS 中的应用

RTPSMessageGroup 被广泛用于：
- 心跳(Heartbeat)消息的构造和发送
- 数据(DATA)消息的批量处理
- GAP 消息的生成
- ACKNACK 消息的处理

## 性能优势

通过使用 RTPSMessageGroup，Fast DDS 能够：
- 减少网络数据包数量
- 提高网络利用率
- 降低系统开销
- 实现更高效的通信

## 示例代码片段

```cpp
// 创建消息组
RTPSMessageGroup group(
    true,  // endianness
    1500,  // max_payload_size (MTU)
    participant,
    loc_selector,
    steady_clock::now() + seconds(1));

// 添加子消息
SubmessageHeader subheader;
// ... 初始化 subheader ...
group.add_submessage(subheader, data, data_size);

// 添加更多子消息...
// group.add_submessage(...);

// 显式刷新或等待自动发送
group.flush();
```

RTPSMessageGroup 是 Fast DDS 实现高效消息传输的核心组件之一，理解它的工作原理对于深入掌握 Fast DDS 的内部机制非常重要。