#include "ListVisitor.hpp"
#include <iostream>

ListVisitor::ListVisitor() {}

ListVisitor::~ListVisitor() {}

void ListVisitor::visit(File* file) {
    std::cout << currentdir << "/" << *file << '\n';
}

void ListVisitor::visit(Directory* directory) {
    std::cout << currentdir << "/" << *directory << '\n';
    auto savedir = currentdir;
    currentdir = currentdir + "/" + directory->getName();
    for (auto entry : *directory) {
        entry->accept(this);
    }
    currentdir = savedir;
}

