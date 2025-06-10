# Fast DDS 中的 ReaderProxy 类型说明

ReaderProxy 是 Fast DDS (也称为 FastRTPS) 中的一个重要类，它用于在发布者端(Publisher)维护关于订阅者(Subscriber)的信息。以下是关于 ReaderProxy 的详细说明：

## 基本概念

ReaderProxy 是 RTPSWriter 内部用来跟踪匹配的 RTPSReader 状态的数据结构。它代表了远端读取器(Reader)在本地写入器(Writer)中的代理。

## 主要功能

1. **状态跟踪**：维护远端 Reader 的接收状态
2. **ACK/NACK 处理**：处理来自 Reader 的确认和否定确认
3. **心跳响应**：处理来自 Reader 的心跳响应
4. **GAP 消息生成**：确定需要发送给 Reader 的 GAP 消息

## 关键成员变量

- `remote_reader_guid_`：远端 Reader 的 GUID
- `changes_for_reader_`：为该 Reader 准备的变更列表
- `requested_changes_set_`：Reader 请求的变更集合
- `acked_changes_set_`：Reader 已确认的变更集合
- `last_acknownledged_sequence_number_`：最后确认的序列号
- `expects_inline_qos_`：指示 Reader 是否期望内联 QoS

## 重要方法

1. `add_change()`：添加变更到代理
2. `change_is_acked()`：检查变更是否被确认
3. `change_is_unsent()`：检查变更是否未发送
4. `requested_changes_set()`：获取请求的变更集合
5. `acked_changes_set()`：获取已确认的变更集合

## 在 Fast DDS 中的作用

ReaderProxy 是实现可靠通信的关键组件，它帮助 Writer 跟踪：
- 哪些数据已经成功传输到 Reader
- 哪些数据需要重传
- Reader 的状态和需求

## 使用场景

ReaderProxy 主要在以下情况下被使用：
- 当 Writer 收到来自 Reader 的 ACK/NACK 时
- 当 Writer 需要确定发送哪些数据给 Reader 时
- 在心跳和 GAP 消息的处理过程中

ReaderProxy 是 Fast DDS 实现 RTPS 协议的重要组成部分，对于理解 Fast DDS 的内部工作机制非常有帮助。