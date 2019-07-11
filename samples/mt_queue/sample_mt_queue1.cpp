/** \example mt_queue/sample_mt_queue1.cpp
 * This is an example of how to use the MTQueue class.
 */
#include <iostream>
#include <cstdio>
#include <thread>

#include "MTQueue.hpp"

using namespace MiniUtils; 

void push(MTQueue<int>* q)
{
    for (int i = 0; i < 10; ++i) {
        //std::cout << "pushing " << i << std::endl;
        printf("pushing %d\n", i);
        q->push(i);
    }
}

void pop(MTQueue<int>* q)
{
    for (int i = 0; i < 10; ++i) {
        printf("poping %d\n", q->pop());
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

