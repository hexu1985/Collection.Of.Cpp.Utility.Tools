#include <vector>
#include <iostream>
#include <fastcdr/FastBuffer.h>
#include <fastcdr/Cdr.h>

class Operation
{
public:
    enum OPERATIONTYPE : uint32_t
    {
        ADDITION = 0,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
    };

    OPERATIONTYPE m_operationType;
    int32_t m_num1;
    int32_t m_num2;

    // 默认构造函数
    Operation() : m_operationType(ADDITION), m_num1(0), m_num2(0) {}

    // Fast CDR 序列化方法
    void serialize(eprosima::fastcdr::Cdr &scdr) const
    {
        // 枚举转换为 uint32_t 进行序列化
        uint32_t type = static_cast<uint32_t>(m_operationType);
        scdr << type;
        scdr << m_num1;
        scdr << m_num2;
    }

    // Fast CDR 反序列化方法
    void deserialize(eprosima::fastcdr::Cdr &dcdr)
    {
        uint32_t type;
        dcdr >> type;
        m_operationType = static_cast<OPERATIONTYPE>(type);
        dcdr >> m_num1;
        dcdr >> m_num2;
    }

    // 计算序列化后的大小（不包含封装头部）
    size_t getCdrSerializedSize() const
    {
        size_t current_alignment = 0;
        size_t calculated_size = 0;

        // m_operationType (uint32_t)
        calculated_size += eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 4;
        current_alignment += 4;

        // m_num1 (int32_t)
        calculated_size += eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 4;
        current_alignment += 4;

        // m_num2 (int32_t)
        calculated_size += eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 4;
        current_alignment += 4;

        return calculated_size;
    }
};

// 序列化函数：将 Operation 存入 vector<uint8_t>
std::vector<uint8_t> serializeOperation(const Operation& op)
{
    // 计算序列化所需大小（包括4字节封装头部）
    size_t data_size = op.getCdrSerializedSize();
    std::cout << "data_size: " << data_size << std::endl;

    // 预分配 vector 空间
    std::vector<uint8_t> buffer;
    buffer.resize(data_size);

    // 使用 FastBuffer 和 Cdr 进行序列化
    eprosima::fastcdr::FastBuffer fastbuffer((char*) buffer.data(), buffer.size());
    
    // 修正1: 使用默认构造函数，不指定 ENDIANNESS 和 Version
    eprosima::fastcdr::Cdr ser(fastbuffer);

    // 序列化数据
    op.serialize(ser);

    // 获取实际写入大小
    size_t actual_size = ser.getSerializedDataLength();
    std::cout << "actual_size: " << actual_size << std::endl;

    // 如果实际大小小于预留大小，调整 vector
    if (actual_size < buffer.size())
    {
        buffer.resize(actual_size);
    }

    return buffer;
}

// 反序列化函数：从 vector<uint8_t> 恢复 Operation
bool deserializeOperation(const std::vector<uint8_t>& buffer, Operation& op)
{
    if (buffer.size() < 4)  // 至少需要4字节的封装头部
    {
        return false;
    }

    eprosima::fastcdr::FastBuffer fastbuffer((char *) buffer.data(), buffer.size());
    
    // 修正2: 使用默认构造函数
    eprosima::fastcdr::Cdr deser(fastbuffer);

    // 在较新版本的 Fast CDR 中，封装头部会自动处理
    // 不需要显式调用 deserialize_encapsulation()
    // 直接反序列化数据即可

    // 反序列化数据
    try
    {
        op.deserialize(deser);
    }
    catch (const std::exception& e)
    {
        std::cerr << "反序列化异常: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "反序列化未知异常" << std::endl;
        return false;
    }

    return true;
}

// 或者，为了更好的兼容性，可以直接在外部处理反序列化
bool deserializeOperationDirect(const std::vector<uint8_t>& buffer, Operation& op)
{
    if (buffer.size() < 4)
    {
        return false;
    }

    eprosima::fastcdr::FastBuffer fastbuffer((char*) buffer.data(), buffer.size());
    eprosima::fastcdr::Cdr deser(fastbuffer);

    try
    {
        // 直接反序列化各个字段
        uint32_t type;
        deser >> type;
        op.m_operationType = static_cast<Operation::OPERATIONTYPE>(type);
        deser >> op.m_num1;
        deser >> op.m_num2;
    }
    catch (const std::exception& e)
    {
        std::cerr << "反序列化异常: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "反序列化未知异常" << std::endl;
        return false;
    }

    return true;
}

void printOperation(const Operation& op, const std::string& prefix)
{
    const char* typeNames[] = {"ADDITION", "SUBTRACTION", "MULTIPLICATION", "DIVISION"};
    std::cout << prefix << " Operation: " 
              << typeNames[op.m_operationType] 
              << " (" << op.m_num1 << " " 
              << (op.m_operationType == Operation::ADDITION ? "+" :
                  op.m_operationType == Operation::SUBTRACTION ? "-" :
                  op.m_operationType == Operation::MULTIPLICATION ? "*" : "/")
              << " " << op.m_num2 << ")" << std::endl;
}

int main()
{
    // 1. 创建并初始化 Operation 对象
    Operation original;
    original.m_operationType = Operation::MULTIPLICATION;
    original.m_num1 = 12;
    original.m_num2 = 7;

    printOperation(original, "原始数据:");

    // 2. 序列化到 vector<uint8_t>
    std::vector<uint8_t> serialized = serializeOperation(original);
    
    std::cout << "序列化后大小: " << serialized.size() << " 字节" << std::endl;
    std::cout << "Vector 容量: " << serialized.capacity() << " 字节" << std::endl;
    
    // 打印十六进制内容
    std::cout << "十六进制数据: ";
    for (auto byte : serialized)
    {
        printf("%02X ", byte);
    }
    std::cout << std::endl;

    // 3. 反序列化 - 使用方法1（使用类的 deserialize 方法）
    Operation restored;
    if (deserializeOperation(serialized, restored))
    {
        printOperation(restored, "恢复后的数据:");
        
        // 验证数据是否一致
        bool success = (original.m_operationType == restored.m_operationType &&
                       original.m_num1 == restored.m_num1 &&
                       original.m_num2 == restored.m_num2);
        std::cout << "数据验证: " << (success ? "✅ 成功" : "❌ 失败") << std::endl;
    }
    else
    {
        std::cout << "反序列化失败(方法1)!" << std::endl;
    }

    // 4. 反序列化 - 使用方法2（直接反序列化，更可靠）
    Operation restored2;
    if (deserializeOperationDirect(serialized, restored2))
    {
        printOperation(restored2, "恢复后的数据(方法2):");
        
        // 验证数据是否一致
        bool success = (original.m_operationType == restored2.m_operationType &&
                       original.m_num1 == restored2.m_num1 &&
                       original.m_num2 == restored2.m_num2);
        std::cout << "数据验证: " << (success ? "✅ 成功" : "❌ 失败") << std::endl;
    }
    else
    {
        std::cout << "反序列化失败(方法2)!" << std::endl;
    }

    return 0;
}
