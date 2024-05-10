#include "Directory.hpp"
#include "Visitor.hpp"

Directory::Directory(const std::string& name_) : name(name_) {}

Directory::~Directory() {}

std::string Directory::getName() {
    return name;
}

int Directory::getSize() {
    int size = 0;
    for (auto entry: dir) {
        size += entry->getSize();
    }
    return size;
}

Entry* Directory::add(Entry* entry) {
    dir.push_back(entry);
    return this;
}

void Directory::accept(Visitor* v) {
    v->visit(this);
}
