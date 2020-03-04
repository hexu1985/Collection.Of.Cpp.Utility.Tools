/** \example mt_queue/sample_mt_queue2.cpp
 * This is an example of how to use the MTQueue class.
 */
#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>

#include "MTQueue.hpp"

using namespace mini_utils; 

void push(MTQueue<int>* q)
{
    for (int i = 0; i < 1000; ++i) {
        q->push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void pop(MTQueue<int>* q)
{
    size_t queue_size = 0;
    while (true) {
        queue_size = q->size();
        std::cout << "queue size: " << queue_size << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (queue_size == 1000)
            break;
    }
}
    
int main()
{
    MTQueue<int> q;
    std::thread t1(push,&q);
    std::thread t2(pop,&q);
    t1.join();
    t2.join();
    return 0;
}

