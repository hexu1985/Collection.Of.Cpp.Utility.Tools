# Fast DDS 中 Sample 与 Change 的关系

在 Fast DDS (DDS 实现) 中，**Sample** 和 **Change** 是两个密切相关但不同的概念：

## 1. 基本定义

- **Sample (样本)**：
  - 是用户层面的数据单元，代表一个数据值
  - 对应应用程序发布或订阅的实际数据内容
  - 例如：传感器读数、控制命令等

- **Change (变更)**：
  - 是 RTPS (Real-Time Publish Subscribe) 协议层面的概念
  - 代表数据状态的一次变更记录
  - 包含 Sample 数据及其元数据(序列号、时间戳等)

## 2. 核心关系

1. **对应关系**：
   - 每个用户发布的 Sample 会在 RTPS 层生成一个对应的 Change
   - Change = Sample + RTPS 协议所需的元数据

2. **生命周期**：
   - 用户创建 Sample → Fast DDS 转换为 Change → 通过网络传输 → 接收方将 Change 转换回 Sample

3. **存储方式**：
   - Samples 存储在 DataWriter/DataReader 的历史缓存中
   - Changes 存储在 RTPS Writer/Reader 的历史缓存中

## 3. 技术实现差异

| 特性        | Sample                          | Change                          |
|-----------|---------------------------------|---------------------------------|
| 层级        | 用户层/DDS层                     | 协议层/RTPS层                    |
| 内容        | 纯应用数据                       | 数据+序列号+时间戳+状态等信息        |
| 可见性       | 对应用程序可见                    | 对协议实现可见，通常对应用透明         |
| 生命周期      | 由用户控制                        | 由RTPS协议管理                    |

## 4. 实际应用中的表现

1. **发布流程**：
   ```cpp
   MyType data; // 创建sample
   writer->write(&data); // 内部转换为change并传输
   ```

2. **QoS影响**：
   - History QoS 控制保留的 samples 数量
   - Resource Limits QoS 控制内存中 changes 的数量

3. **性能考量**：
   - 大量小 samples → 产生大量 changes → 增加协议开销
   - 大 samples → 单个 change 体积大 → 可能影响实时性

## 5. 调试观察

可以通过 Fast DDS 日志查看 changes 的流动：
```cpp
Log::SetVerbosity(Log::Kind::Info);
Log::SetCategoryFilter(std::regex("(RTPS_CDR_MSG)"));
```

理解 sample 和 change 的区别有助于：
- 更精确地配置 QoS 参数
- 更好地诊断通信问题
- 优化系统性能