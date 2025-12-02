#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <cstring>

namespace bip = boost::interprocess;

// 写入进程
void writer_process() {
    try {
        // 删除可能存在的共享内存
        bip::shared_memory_object::remove("MySharedMemory");
        
        // 创建托管共享内存（64KB）
        bip::managed_shared_memory segment(
            bip::create_only,
            "MySharedMemory",
            65536
        );
        
        // 构造一个整数
        int *p = segment.construct<int>("MyInteger")(42);
        std::cout << "Writer: 设置整数 = " << *p << std::endl;
        
        // 构造一个字符数组
        char *str = segment.construct<char>("MyString")[100](); // 100个字符
        std::strcpy(str, "Hello from shared memory!");
        std::cout << "Writer: 设置字符串 = " << str << std::endl;
        
        // 保持共享内存存在
        std::cout << "\n按Enter键继续...";
        std::cin.get();
        
        // 清理（可选）
        segment.destroy<int>("MyInteger");
        segment.destroy<char>("MyString");
    }
    catch(const bip::interprocess_exception& ex) {
        std::cerr << "Writer错误: " << ex.what() << std::endl;
    }
}

// 读取进程
void reader_process() {
    try {
        // 打开已存在的共享内存
        bip::managed_shared_memory segment(
            bip::open_only,
            "MySharedMemory"
        );
        
        // 查找整数
        std::pair<int*, size_t> ret_int = segment.find<int>("MyInteger");
        if(ret_int.first) {
            std::cout << "Reader: 找到整数 = " << *ret_int.first << std::endl;
        }
        
        // 查找字符串
        std::pair<char*, size_t> ret_str = segment.find<char>("MyString");
        if(ret_str.first) {
            std::cout << "Reader: 找到字符串 = " << ret_str.first << std::endl;
        }
    }
    catch(const bip::interprocess_exception& ex) {
        std::cerr << "Reader错误: " << ex.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc > 1 && std::strcmp(argv[1], "reader") == 0) {
        reader_process();
    } else {
        writer_process();
    }
    return 0;
}