#include "Entry.hpp"
#include <iostream>

Entry::~Entry() {}

std::string Entry::toString() {
    return getName() + " (" + std::to_string(getSize()) + ")";
}

std::ostream& operator<<(std::ostream& out, Entry& entry) {
    out << entry.toString();
    return out;
}
