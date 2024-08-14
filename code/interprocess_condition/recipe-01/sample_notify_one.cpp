// condition_variable::notify_one
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::unique_lock
#include "interprocess_mutex.hpp"
#include "interprocess_condition.hpp" 

InterprocessMutex mtx;
InterprocessCondition produce,consume;

int cargo = 0;     // shared value by producers and consumers

void consumer () {
    std::unique_lock<InterprocessMutex> lck(mtx);
    while (cargo==0) consume.wait(lck);
    std::cout << cargo << '\n';
    cargo=0;
    produce.notify_one();
}

void producer (int id) {
    std::unique_lock<InterprocessMutex> lck(mtx);
    while (cargo!=0) produce.wait(lck);
    cargo = id;
    consume.notify_one();
}

int main ()
{
    std::thread consumers[10],producers[10];
    // spawn 10 consumers and 10 producers:
    for (int i=0; i<10; ++i) {
        consumers[i] = std::thread(consumer);
        producers[i] = std::thread(producer,i+1);
    }

    // join them back:
    for (int i=0; i<10; ++i) {
        producers[i].join();
        consumers[i].join();
    }

    return 0;
}
