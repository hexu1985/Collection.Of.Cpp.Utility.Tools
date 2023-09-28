#include "Timer.hpp"
#include <ctime>
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>
#include <chrono>
#include <iomanip>

using threading::Timer;

struct Alarm {
    Alarm(double seconds_, const std::string& message_): 
        seconds(seconds_), message(message_) {
    }

    double seconds;
    std::string message;
};

std::string strftime(const char* format, const std::chrono::time_point<std::chrono::system_clock>& tp) {
    time_t rawtime = std::chrono::system_clock::to_time_t(tp);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format, localtime(&rawtime));
    return std::string(mbstr);
}

std::ostream& operator<<(std::ostream& out, const std::chrono::time_point<std::chrono::system_clock>& tp) {
    auto cs = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count() % 1000000;
    out << strftime("%Y-%m-%d %H:%M:%S", tp) << '.' << std::setfill('0') << std::setw(6) << cs << std::setfill(' ');
    return out;
}

std::ostream& operator<<(std::ostream& out, const Alarm& alarm) {
    out << "(" << alarm.seconds << ") " << alarm.message;
    return out;
}

void callback(std::shared_ptr<Alarm> alarm) {
    std::cout << "\nalarm timer [" << *alarm << "] at " << std::chrono::system_clock::now() << std::endl;
}

std::tuple<double, std::string> parse_command(const std::string& line) {
    auto pos = line.find(' ');
    if (pos == std::string::npos)
        throw std::runtime_error("invalid line: separator not found");

    double seconds = std::stod(line.substr(0, pos));
    std::string message = line.substr(pos+1);
    return std::make_tuple(seconds, message);
}

int main()
{
    std::string line;
    double seconds;
    std::string message;
    while (true) {
        std::cout << "Alarm> ";
        if (!std::getline(std::cin, line)) exit(0);
        if (line.length() <= 1) continue;

        try {
            std::tie(seconds, message) = parse_command(line);
            auto alarm = std::make_shared<Alarm>(seconds, message);
            std::cout << "start timer [" << *alarm << "] at " << std::chrono::system_clock::now() << std::endl;
            auto t = Timer(seconds, std::bind(callback, alarm));
#ifdef DEBUG
            t.set_message(message);
#endif
            t.start();
        } 
        catch (const std::exception& e) {
            std::cout << "Bad command" << std::endl;
        }
    }
}

