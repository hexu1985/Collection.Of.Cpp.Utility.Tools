#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include <string>
#include <cstring>  // for memcpy

int main()
{
    char buffer[256];

    // 序列化数据
    eprosima::fastcdr::FastBuffer fast_buffer(buffer, sizeof(buffer));
    eprosima::fastcdr::Cdr cdr(fast_buffer);
    cdr << (uint32_t)42 << (float)3.14;

    std::string serialized_data(
        reinterpret_cast<char*>(fast_buffer.getBuffer()),
        fast_buffer.getBufferSize()
    );

    eprosima::fastcdr::FastBuffer fast_buffer2(
            &serialized_data[0],
            serialized_data.size()
            );
    eprosima::fastcdr::Cdr cdr2(fast_buffer2);

    uint32_t value1;
    float value2;
    cdr >> value1 >> value2;

    std::cout << "value1: " << value1 << ", "
              << "value2: " << value2 << std::endl;

    return 0;
}

