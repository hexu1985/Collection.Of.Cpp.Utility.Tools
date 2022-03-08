/**
 * @file memory_pool.hpp
 * @brief 固定大小对象的内存池
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-25
 *
 * @see 提高C++性能的编程技术, 第6.3章节
 */
#ifndef MEMORY_POOL_INC
#define MEMORY_POOL_INC

#include <cstddef>
#include <new>

/**
 * @brief 固定大小对象的内存池
 *
 * @tparam T 元素类型
 */
template <class T>
class MemoryPool {
public:
    /**
     * @brief 创建一个内存池对象
     *
     * @param size 每次空闲列表空时, 扩展空闲列表的块个数
     */
	MemoryPool(size_t size = EXPANSION_SIZE);

	virtual ~MemoryPool();

    /**
     * @brief 从内存池中分配sizeof(T)大小的内存
     *
     * @param size 未使用
     *
     * @return 返回分配内存的指针
     */
	inline void *alloc(size_t size);

    /**
     * @brief 释放先前从内存池中分配的内存
     *
     * @param someElement 被释放的指针
     */
	inline void free(void *someElement);

private:
    // 内存块结构
    struct MemoryChunk {
        MemoryChunk *next;
    };

    MemoryChunk *freeList = NULL;    // 空闲列表的下一元素
	
	// 如果空闲列表为空, 按该大小扩展它
	enum { EXPANSION_SIZE = 32 };

	// 添加空闲元素至空闲列表
	void expandTheFreeList(int howMany = EXPANSION_SIZE);
};

template <class T>
MemoryPool<T>::MemoryPool(size_t size)
{
	expandTheFreeList(size);
}

template <class T>
MemoryPool<T>::~MemoryPool()
{
	for (MemoryChunk *nextPtr = freeList; freeList != NULL; nextPtr = freeList) {
		freeList = freeList->next;
		delete [] nextPtr;
	}
}

template <class T>
inline
void *MemoryPool<T>::alloc(size_t)
{
	if (!freeList) {
		expandTheFreeList();
	}

	MemoryChunk *head = freeList;
	freeList = head->next;

	return head;
}

template <class T>
inline
void MemoryPool<T>::free(void *doomed)
{
	MemoryChunk *head = static_cast<MemoryChunk *>(doomed);

	head->next = freeList;
	freeList = head;
}

template <class T>
void MemoryPool<T>::expandTheFreeList(int howMany)
{
	// 我们必须分配足够大的对象以包含下一个指针
	size_t size = (sizeof(T) > sizeof(MemoryChunk *)) ?
		sizeof(T) : sizeof(MemoryChunk *);

	MemoryChunk *runner = reinterpret_cast<MemoryChunk *>(new char[size]);

	freeList = runner;
	for (int i = 0; i < howMany; i++) {
		runner->next =
			reinterpret_cast<MemoryChunk *>(new char[size]);
		runner = runner->next;
	}
	runner->next = NULL;
}

#endif
