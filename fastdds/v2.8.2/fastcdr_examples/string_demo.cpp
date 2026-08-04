#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include <string>
#include <cstring>  // for memcpy

int main()
{
    // 1. 准备缓冲区
    std::string serialized_data;

    // 2. 序列化
    {
        char buffer[256];

        eprosima::fastcdr::FastBuffer fast_buffer(buffer, sizeof(buffer));
        eprosima::fastcdr::Cdr cdr(fast_buffer);
        cdr << (uint32_t)42 << (float)3.14;

        serialized_data = std::string(
                reinterpret_cast<char*>(fast_buffer.getBuffer()),
                fast_buffer.getBufferSize()
                );
    }

    // 3. 反序列化
    {
        eprosima::fastcdr::FastBuffer fast_buffer(
                &serialized_data[0],
                serialized_data.size()
                );
        eprosima::fastcdr::Cdr cdr(fast_buffer);

        uint32_t value1;
        float value2;
        cdr >> value1 >> value2;

        std::cout << "value1: " << value1 << ", "
            << "value2: " << value2 << std::endl;
    }

    return 0;
}

