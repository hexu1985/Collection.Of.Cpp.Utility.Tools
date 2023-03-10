#pragma once

#include <string>

bool starts_with(const std::string& input, const std::string& test) {
    return input.compare(0, test.length(), test) == 0;
}

