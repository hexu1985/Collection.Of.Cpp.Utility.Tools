#pragma once

#include <thread>
#include <atomic>

class Thread {
private:
    std::thread thread_;

public:
    Thread(); 
    virtual ~Thread();

    void start();
    void join();
    
protected:
    virtual void run() = 0;
};
