#include "thread.hpp"
#include "random.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include <sys/types.h>
#include <unistd.h>

std::mutex threadLock{};

class MyThreadClass: public Thread {
private:
    std::string name;
    int duration;

public:
    MyThreadClass(const std::string& name, int duration);
    MyThreadClass(MyThreadClass&&) = default;
    ~MyThreadClass();
    void run() override;
};

MyThreadClass::MyThreadClass(const std::string& name_, int duration_):
    name(name_), duration(duration_) {
}

MyThreadClass::~MyThreadClass() {
}

void MyThreadClass::run() {
    threadLock.lock();
    std::cout << "---> " << name 
        << " running, belonging to process ID "
        << getpid() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    std::cout << "---> " << name << " over" << std::endl;
    threadLock.unlock();
}

int main(int argc, char* argv[]) {
    auto start_time = std::chrono::system_clock::now();

    // Thread Creation
    auto thread1 = MyThreadClass("Thread#1 ", randint(1,10));
    auto thread2 = MyThreadClass("Thread#2 ", randint(1,10));
    auto thread3 = MyThreadClass("Thread#3 ", randint(1,10));
    auto thread4 = MyThreadClass("Thread#4 ", randint(1,10));
    auto thread5 = MyThreadClass("Thread#5 ", randint(1,10));
    auto thread6 = MyThreadClass("Thread#6 ", randint(1,10));
    auto thread7 = MyThreadClass("Thread#7 ", randint(1,10));
    auto thread8 = MyThreadClass("Thread#8 ", randint(1,10));
    auto thread9 = MyThreadClass("Thread#9 ", randint(1,10));

    // Thread Running
    thread1.start();
    thread2.start();
    thread3.start();
    thread4.start();
    thread5.start();
    thread6.start();
    thread7.start();
    thread8.start();
    thread9.start();

    // Thread joining
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();

    // End 
    std::cout << "End" << std::endl;

    // Execution Time
    std::cout << "--- " 
        << std::chrono::duration<double>(std::chrono::system_clock::now() - start_time).count()
        << " seconds ---" << std::endl;
}
