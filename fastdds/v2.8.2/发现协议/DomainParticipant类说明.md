# DomainParticipant 类说明 (Fast DDS)

DomainParticipant 是 Fast DDS (Fast RTPS) 中的核心类，代表一个 DDS 域的参与者，是创建发布者(Publisher)、订阅者(Subscriber)、数据写入者(DataWriter)和数据读取者(DataReader)的工厂类。

## 类概述

DomainParticipant 是 DDS 域中的通信端点，负责：
- 管理域内的实体创建和销毁
- 处理发现过程
- 提供对域级别 QoS 策略的访问
- 作为主题(Topic)创建的工厂

## 主要功能

1. **实体管理**：创建和删除 Publisher、Subscriber、Topic 等实体
2. **域配置**：管理域级别的 QoS 策略
3. **发现服务**：参与域内实体的自动发现过程
4. **监听机制**：提供域级别的事件通知

## 关键方法

### 实体创建方法

```cpp
// 创建 Publisher
Publisher* create_publisher(const PublisherQos& qos,
                           PublisherListener* listener = nullptr);

// 创建 Subscriber
Subscriber* create_subscriber(const SubscriberQos& qos,
                             SubscriberListener* listener = nullptr);

// 创建 Topic
Topic* create_topic(const std::string& topic_name,
                   const std::string& type_name,
                   const TopicQos& qos,
                   TopicListener* listener = nullptr);

// 创建 DataWriter
DataWriter* create_datawriter(Topic* topic,
                             const DataWriterQos& qos,
                             DataWriterListener* listener = nullptr);

// 创建 DataReader
DataReader* create_datareader(TopicDescription* topic,
                             const DataReaderQos& qos,
                             DataReaderListener* listener = nullptr);
```

### 实体删除方法

```cpp
ReturnCode_t delete_publisher(Publisher* publisher);
ReturnCode_t delete_subscriber(Subscriber* subscriber);
ReturnCode_t delete_topic(Topic* topic);
ReturnCode_t delete_datawriter(DataWriter* writer);
ReturnCode_t delete_datareader(DataReader* reader);
```

### 查询方法

```cpp
// 获取域ID
DomainId_t get_domain_id() const;

// 查找已存在的 Topic
Topic* find_topic(const std::string& topic_name,
                 const Duration_t& timeout);

// 获取参与者 QoS
const DomainParticipantQos& get_qos() const;
```

### QoS 配置方法

```cpp
ReturnCode_t set_qos(const DomainParticipantQos& qos);
```

## 重要成员变量

```cpp
DomainId_t domain_id_;  // 参与的域ID
DomainParticipantQos qos_;  // QoS设置
ParticipantListener* listener_;  // 监听器
```

## QoS 策略

DomainParticipantQos 包含以下重要策略：
- `user_data`：附加的用户数据
- `entity_factory`：控制实体创建行为
- `wire_protocol`：配置RTPS协议参数
- `transport`：配置传输层
- `name`：参与者名称

## 使用示例

```cpp
// 创建参与者
DomainParticipant* participant =
    DomainParticipantFactory::get_instance()->create_participant(0);

// 创建发布者
Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);

// 创建主题
Topic* topic = participant->create_topic("ExampleTopic", "ExampleType", TOPIC_QOS_DEFAULT);

// 创建DataWriter
DataWriter* writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT);
```

## 生命周期管理

1. 通过 DomainParticipantFactory 创建
2. 使用完毕后必须通过 DomainParticipantFactory 删除
3. 删除参与者会自动删除其创建的所有子实体

## 注意事项

- 每个 DomainParticipant 只能属于一个域
- 删除参与者前应先删除所有子实体
- QoS 设置通常在创建时指定，后期修改可能受限
- 监听器回调在参与者线程中执行，不应执行耗时操作

## 相关类

- DomainParticipantFactory：参与者工厂类
- Publisher/Subscriber：发布/订阅实体
- Topic：主题实体
- DataWriter/DataReader：数据读写实体

DomainParticipant 是 Fast DDS 应用程序的入口点，正确使用它是构建 DDS 系统的关键第一步。
