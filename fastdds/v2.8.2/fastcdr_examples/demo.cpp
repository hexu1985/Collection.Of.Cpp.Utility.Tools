#include <fastcdr/Cdr.h>
#include <fastcdr/FastCdr.h>

int main()
{
    // 1. 准备缓冲区
    char buffer[256];

    // 2. 序列化
    {
        eprosima::fastcdr::FastBuffer cdr_buffer(buffer, sizeof(buffer));
        eprosima::fastcdr::Cdr cdr_ser(cdr_buffer);

        uint32_t id = 1234;
        float value = 3.14f;
        std::string name = "hello";

        cdr_ser << id << value << name;
        // 现在 buffer 里就是序列化后的字节流
    }

    // 3. 反序列化
    {
        eprosima::fastcdr::FastBuffer cdr_buffer(buffer, sizeof(buffer));
        eprosima::fastcdr::Cdr cdr_deser(cdr_buffer);

        uint32_t id_out;
        float value_out;
        std::string name_out;

        cdr_deser >> id_out >> value_out >> name_out;
        // 反序列化完成，数据已恢复
    }

    return 0;
}
