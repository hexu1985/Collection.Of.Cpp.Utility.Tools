#pragma once

#include <functional>
#include <memory>

namespace threading {

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(double interval, Callback function); 
    void start();
    void cancel(); 

public:
    struct Impl; 

private:
    std::shared_ptr<Impl> pimpl;
};

}   // namespace threading
