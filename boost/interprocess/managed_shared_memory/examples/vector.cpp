#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <iostream>

int main() {
    using namespace boost::interprocess;
    
    // 别名定义
    typedef allocator<int, managed_shared_memory::segment_manager> 
            ShmemAllocator;
    typedef vector<int, ShmemAllocator> MyVector;
    
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
        
        // 模拟另一个进程读取
        managed_shared_memory segment2(open_only, "MySharedMemory3");
        std::pair<MyVector*, std::size_t> ret = segment2.find<MyVector>("MyVector");
        
        if(ret.first) {
            std::cout << "Another process read vector size: " 
                      << ret.first->size() << std::endl;
        }
        
        // 清理
        segment.destroy<MyVector>("MyVector");
        shared_memory_object::remove("MySharedMemory3");
        
    } catch(interprocess_exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    
    return 0;
}
