#include "timer.hpp"
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

struct Alarm {
    Alarm(double seconds_, const std::string& message_): 
        seconds(seconds_), message(message_) {
    }

    double seconds;
    std::string message;
};

void callback(std::shared_ptr<Alarm> alarm) {
    std::cout << "(" << alarm->seconds << ") " << alarm->message << std::endl;
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
            Timer t;
            t.setTimeout(std::bind(callback, alarm), seconds*1000);
        } 
        catch (const std::exception& e) {
            std::cout << "Bad command" << std::endl;
        }
    }
}

