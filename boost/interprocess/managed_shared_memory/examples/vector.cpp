#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <iostream>

using namespace boost::interprocess;

// 别名定义
typedef allocator<int, managed_shared_memory::segment_manager> 
        ShmemAllocator;
typedef vector<int, ShmemAllocator> MyVector;
    
// 写入进程
void writer_process() {
    shared_memory_object::remove("MySharedMemory3");
    
    try {
        managed_shared_memory segment(create_only, "MySharedMemory3", 65536);
        
        // 初始化allocator
        const ShmemAllocator alloc_inst(segment.get_segment_manager());
        
        // 在共享内存中构造vector
        MyVector *myvector = segment.construct<MyVector>("MyVector")(alloc_inst);
        
        // 向vector中添加数据
        for(int i = 0; i < 10; ++i) {
            myvector->push_back(i * 10);
        }
        
        // 读取数据
        std::cout << "Vector contents: ";
        for(size_t i = 0; i < myvector->size(); ++i) {
            std::cout << (*myvector)[i] << " ";
        }
        std::cout << std::endl;

        // 保持共享内存存在
        std::cout << "\n按Enter键继续...";
        std::cin.get();

        // 清理
        segment.destroy<MyVector>("MyVector");
        shared_memory_object::remove("MySharedMemory3");
        
    } catch(interprocess_exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return;
    }
}
    
// 读取进程
void reader_process() {
    try {
        // 另一个进程读取
        managed_shared_memory segment2(open_only, "MySharedMemory3");
        std::pair<MyVector*, std::size_t> ret = segment2.find<MyVector>("MyVector");

        if(!ret.first) {
            std::cout << "did not find MyVector" << std::endl;
            return;
        }

        MyVector* myvector = ret.first;
        std::cout << "Another process read vector size: " 
            << myvector->size() << std::endl;
        
        // 读取数据
        std::cout << "Vector contents: ";
        for(size_t i = 0; i < myvector->size(); ++i) {
            std::cout << (*myvector)[i] << " ";
        }
        std::cout << std::endl;

    } catch(interprocess_exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return;
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
