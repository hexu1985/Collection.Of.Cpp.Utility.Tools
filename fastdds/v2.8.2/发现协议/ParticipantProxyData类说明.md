# ParticipantProxyData 类说明 (Fast DDS)

`ParticipantProxyData` 是 Fast DDS (formerly Fast RTPS) 中的一个重要类，用于表示远程参与者的信息。以下是关于这个类的详细说明：

## 基本概念

`ParticipantProxyData` 类用于存储和管理与 DDS 域中远程参与者(DomainParticipant)相关的所有信息。当本地参与者发现远程参与者时，会创建并维护对应的 `ParticipantProxyData` 对象。

## 主要功能

1. **存储远程参与者信息**：包含远程参与者的标识符、属性、QoS 设置等
2. **发现协议支持**：作为发现协议的一部分，用于交换参与者信息
3. **端点管理**：维护与该参与者关联的所有端点的信息

## 主要成员变量

- `m_protocolVersion`：协议版本
- `m_guid`：参与者的全局唯一标识符(GUID)
- `m_VendorId`：厂商ID
- `m_expectsInlineQos`：是否期望内联QoS
- `m_availableBuiltinEndpoints`：可用的内置端点
- `m_metatrafficUnicastLocatorList`：元数据单播定位器列表
- `m_metatrafficMulticastLocatorList`：元数据多播定位器列表
- `m_defaultUnicastLocatorList`：默认单播定位器列表
- `m_defaultMulticastLocatorList`：默认多播定位器列表
- `m_participantName`：参与者名称
- `m_userData`：用户自定义数据
- `m_properties`：参与者属性
- `m_leaseDuration`：租约持续时间
- `m_isAlive`：参与者是否活跃状态

## 主要方法

1. **构造函数与析构函数**
   - `ParticipantProxyData()`
   - `~ParticipantProxyData()`

2. **信息设置与获取**
   - `set_participant_name()`：设置参与者名称
   - `get_participant_name()`：获取参与者名称
   - `set_lease_duration()`：设置租约持续时间
   - `get_lease_duration()`：获取租约持续时间

3. **序列化与反序列化**
   - `readFromCDRMessage()`：从CDR消息中读取数据
   - `writeToCDRMessage()`：将数据写入CDR消息

4. **端点管理**
   - `addReaderProxyData()`：添加读者代理数据
   - `addWriterProxyData()`：添加写者代理数据
   - `removeReaderProxyData()`：移除读者代理数据
   - `removeWriterProxyData()`：移除写者代理数据

## 使用场景

`ParticipantProxyData` 主要在以下场景中使用：

1. **发现过程**：在参与者发现协议(PDP)中交换信息
2. **端点匹配**：用于确定哪些远程端点可以与本地端点匹配
3. **网络配置**：确定如何与远程参与者建立通信

## 注意事项

1. 通常不需要直接实例化或操作此类，除非实现自定义发现机制
2. 修改此类对象可能影响发现过程和通信行为
3. 在多线程环境中使用时需要注意同步

## 示例代码片段

```cpp
// 通常在发现回调中访问ParticipantProxyData
void MyDiscoveryListener::onParticipantDiscovery(
    eprosima::fastdds::dds::DomainParticipant* participant,
    eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info)
{
    if(info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT)
    {
        // 访问ParticipantProxyData
        const eprosima::fastrtps::rtps::ParticipantProxyData& proxy = info.info;
        
        std::cout << "Discovered participant: " << proxy.m_participantName << std::endl;
        std::cout << "GUID: " << proxy.m_guid << std::endl;
    }
}
```

`ParticipantProxyData` 是 Fast DDS 发现机制的核心组成部分，理解它的结构和功能对于深入掌握 Fast DDS 的工作原理非常重要。
