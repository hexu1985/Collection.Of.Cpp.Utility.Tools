# **SampleLostStatus 详解（Fast DDS / Fast RTPS）**

`SampleLostStatus` 是 Fast DDS 中用于报告 **数据样本丢失情况** 的状态结构体，当 DataReader 检测到数据丢失时（如缓冲区溢出、网络丢包等），会通过此状态通知应用程序。

---

## **1. SampleLostStatus 的核心作用**
### **(1) 数据丢失检测场景**
| 场景 | 可能原因 |
|------|----------|
| **可靠性模式下的样本丢失** | 发送缓冲区满、网络持续中断 |
| **历史缓存溢出** | DataReader 处理速度跟不上写入速度 |
| **QoS 不匹配** | DataWriter 和 DataReader 的 `ReliabilityQosPolicy` 配置冲突 |

### **(2) 状态结构体定义**
```cpp
struct SampleLostStatus {
    uint32_t total_count;         // 累计丢失样本总数
    uint32_t total_count_change;  // 相比上次通知的新增丢失数
};
```

---

## **2. 数据丢失的监听方式**
### **(1) 通过 Listener 回调**
```cpp
class MyListener : public DataReaderListener {
public:
    void on_sample_lost(
        DataReader* reader,
        const SampleLostStatus& status) override {
        
        std::cout << "新增丢失样本: " << status.total_count_change 
                  << ", 总丢失: " << status.total_count << std::endl;
    }
};

// 注册监听器
reader->set_listener(&my_listener);
```

### **(2) 通过 StatusCondition + WaitSet**
```cpp
StatusCondition& condition = reader->get_statuscondition();
condition.set_enabled_statuses(StatusMask::sample_lost());

WaitSet waitset;
waitset.attach_condition(condition);

while (true) {
    ConditionSeq active_conditions;
    if (waitset.wait(active_conditions) == ReturnCode_t::RETCODE_OK) {
        SampleLostStatus status;
        reader->get_sample_lost_status(status);
        // 处理丢失数据...
    }
}
```

---

## **3. 关键问题分析**
### **(1) 数据丢失的常见原因**
| 原因 | 解决方案 |
|------|----------|
| **DataWriter 发送过快** | 增加 DataReader 的 `ResourceLimitsQosPolicy.max_samples` |
| **网络不稳定** | 启用 `RELIABLE_RELIABILITY_QOS` + 重传机制 |
| **DataReader 处理延迟** | 优化处理逻辑或使用多线程 |

### **(2) 与 QoS 策略的关系**
| QoS 策略 | 影响样本丢失的机制 |
|----------|--------------------|
| **`ReliabilityQosPolicy`** | `BEST_EFFORT` 模式允许丢包 |
| **`HistoryQosPolicy`** | `KEEP_LAST` 可能丢弃旧数据 |
| **`ResourceLimitsQosPolicy`** | 缓冲区大小限制直接决定容量 |

---

## **4. 诊断与调试**
### **(1) 获取详细丢失信息**
```cpp
SampleLostStatus status;
reader->get_sample_lost_status(status);

// 结合 SampleInfo 分析
LoanableSequence<MyData> data;
SampleInfoSeq infos;
reader->take(data, infos);

for (size_t i = 0; i < infos.length(); ++i) {
    if (infos[i].sample_state == NOT_READ) {
        std::cout << "未读样本丢失！" << std::endl;
    }
}
```

### **(2) 日志监控**
```bash
# 启用详细日志
export FASTDDS_LOG_LEVEL=INFO
```

---

## **5. 最佳实践**
### **(1) 可靠性配置建议**
```cpp
// DataReader 端确保可靠传输
DataReaderQos reader_qos;
reader_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
reader_qos.history().kind = KEEP_ALL_HISTORY_QOS;
reader_qos.resource_limits().max_samples = 10000;
```

### **(2) 实时监控代码示例**
```cpp
void monitor_sample_loss(DataReader* reader) {
    SampleLostStatus status;
    reader->get_sample_lost_status(status);
    
    if (status.total_count_change > 0) {
        // 触发告警或恢复逻辑
        handle_data_loss(status);
    }
}
```

---

## **6. 总结**
| 关键点 | 说明 |
|--------|------|
| **触发条件** | 数据未及时处理、QoS 不匹配、网络问题 |
| **监听方式** | Listener 回调或 StatusCondition 等待 |
| **关键参数** | `total_count`(总量)、`total_count_change`(增量) |
| **根本解决** | 调整 QoS（可靠性/资源限制）+ 优化处理逻辑 |

`SampleLostStatus` 是 **诊断数据完整性问题** 的重要工具，合理使用可以：
- 及时发现数据传输异常
- 指导 QoS 参数调优
- 提高系统可靠性
