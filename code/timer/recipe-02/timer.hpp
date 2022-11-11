#pragma once

#include <chrono>
#include <functional>
#include <memory>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function); 
    void Start();
    void Cancel(); 

#ifdef DEBUG
    void SetMessage(const std::string& message);
#endif

public:
    struct Impl; 

private:
    std::shared_ptr<Impl> pimpl;
};


