#include "Thread.hpp"
#include "Random.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include <sys/types.h>
#include <unistd.h>

class MyThreadClass: public Thread {
private:
    std::string name;
    int duration;

public:
    MyThreadClass(const std::string& name, int duration);
    ~MyThreadClass();
    void run() override;
};

MyThreadClass::MyThreadClass(const std::string& name_, int duration_):
    name(name_), duration(duration_) {
}

MyThreadClass::~MyThreadClass() {
}

void MyThreadClass::run() {
    std::cout << "---> " << name 
        << " running, belonging to process ID "
        << getpid() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    std::cout << "---> " << name << " over" << std::endl;
}

int main(int argc, char* argv[]) {
    auto start_time = std::chrono::system_clock::now();

    // Thread Creation
    Random random;
    MyThreadClass thread1("Thread#1 ", random.randomInt(1,10));
    MyThreadClass thread2("Thread#2 ", random.randomInt(1,10));
    MyThreadClass thread3("Thread#3 ", random.randomInt(1,10));
    MyThreadClass thread4("Thread#4 ", random.randomInt(1,10));
    MyThreadClass thread5("Thread#5 ", random.randomInt(1,10));
    MyThreadClass thread6("Thread#6 ", random.randomInt(1,10));
    MyThreadClass thread7("Thread#7 ", random.randomInt(1,10));
    MyThreadClass thread8("Thread#8 ", random.randomInt(1,10));
    MyThreadClass thread9("Thread#9 ", random.randomInt(1,10));

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
