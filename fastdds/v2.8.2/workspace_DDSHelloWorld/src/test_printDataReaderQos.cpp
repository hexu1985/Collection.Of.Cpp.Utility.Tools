#include "printDataReaderQos.hpp"

int main()
{
    // 创建 DataReaderQos 实例
    eprosima::fastdds::dds::DataReaderQos qos;
    
#if 0
    // 设置一些 QoS 值（可选）
    qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
    qos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
    qos.history().depth = 10;
#endif
    
    // 打印 QoS
    printDataReaderQos(qos);
    
    return 0;
}
