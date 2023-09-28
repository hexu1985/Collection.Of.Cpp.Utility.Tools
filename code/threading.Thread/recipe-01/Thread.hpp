#pragma once

#include <thread>
#include <atomic>

class Thread {
private:
    std::thread thread;

public:
    Thread(); 
    Thread(Thread&& x); 

    virtual ~Thread();

    void start();
    void join();
    
protected:
    virtual void run() = 0;
};
