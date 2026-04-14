# FastDDS源码剖析：消息收发流程

## 1. FastDDS概述

FastDDS是eProsima公司开发的DDS(Data Distribution Service)实现，遵循RTPS(Real-Time Publish-Subscribe)协议，是ROS2的默认中间件。

## 2. 核心组件

### 2.1 DomainParticipant
- 代表DDS域中的参与者
- 负责创建Publisher和Subscriber

### 2.2 Publisher
- 数据发布者
- 管理DataWriter实例

### 2.3 Subscriber
- 数据订阅者
- 管理DataReader实例

### 2.4 Topic
- 定义数据的类型和名称

## 3. 消息发送流程

### 3.1 数据写入流程
1. **应用层调用**:
```cpp
data_writer->write(&data);
```

2. **DataWriter处理**:
- 在`DataWriterImpl::write()`中准备数据
- 调用`PublisherImpl::prepare_data()`

3. **历史缓存**:
- 数据被添加到`WriterHistory`中
- 在`RTPSWriter::add_change()`中实现

4. **RTPS协议处理**:
- `RTPSWriter`将数据转换为RTPS消息
- 通过`MessageReceiver`处理RTPS子消息

5. **传输层发送**:
- 使用UDP或TCP传输
- 通过`RTPSParticipantImpl::sendSync()`发送
- 最终调用操作系统socket API

### 3.2 关键代码路径
```
DataWriter::write()
→ DataWriterImpl::write()
→ PublisherImpl::prepare_data()
→ RTPSWriter::add_change()
→ RTPSMessageGroup::add_data()
→ RTPSParticipantImpl::sendSync()
→ UDPTransportInterface::send()
→ 系统socket sendto()
```

## 4. 消息接收流程

### 4.1 数据接收流程
1. **网络层接收**:
- 操作系统socket接收数据
- 通过`UDPTransportInterface`回调

2. **RTPS消息解析**:
- `RTPSMessageReceiver`解析RTPS消息
- 识别DATA子消息

3. **ReaderProxy处理**:
- `StatefulReader`或`StatelessReader`处理
- 检查序列号、去重等

4. **历史缓存**:
- 数据添加到`ReaderHistory`
- 在`RTPSReader::processDataMsg()`中实现

5. **数据通知应用**:
- 触发`DataReaderListener`回调
- 应用通过`take()`或`read()`获取数据

### 4.2 关键代码路径
```
系统socket接收
→ UDPTransportInterface::Receive()
→ RTPSParticipantImpl::onNewDataMessage()
→ RTPSMessageGroup::processDataMsg()
→ StatefulReader::processDataMsg()
→ ReaderHistory::add_change()
→ DataReaderImpl::notify()
→ DataReaderListener::on_data_available()
→ DataReader::read()/take()
```

## 5. QoS策略处理

FastDDS在消息收发过程中会考虑以下QoS策略：
- 可靠性(Reliability): BEST_EFFORT或RELIABLE
- 持久性(Durability): VOLATILE, TRANSIENT_LOCAL等
- 截止时间(Deadline)
- 生命周期(Liveliness)
- 流量控制(Flow Controllers)

## 6. 性能优化点

1. **零拷贝机制**:
- 使用`loan_sample()`避免数据复制
- 在`DataWriter`和`DataReader`中实现

2. **异步发布模式**:
- 通过`PublisherQos`配置
- 使用独立线程发送数据

3. **批处理**:
- `RTPSMessageGroup`支持多条消息打包发送

4. **碎片化处理**:
- 大数据自动分片传输
- 在接收端重组

## 7. 调试与分析建议

1. 启用FastDDS日志:
```cpp
Log::SetVerbosity(Log::Kind::Info);
```

2. 使用Wireshark分析RTPS流量:
- 过滤条件: `udp.port == 7411`(默认多播端口)

3. 关键性能计数器:
- 发送/接收消息计数
- 网络延迟统计
- 丢包率

## 8. 总结

FastDDS的消息收发流程经过高度优化，支持实时系统需求。理解其内部机制有助于:
- 性能调优
- 问题诊断
- 定制化开发

核心思想是通过分层架构将应用数据高效转换为RTPS消息，并可靠传输到对端。
