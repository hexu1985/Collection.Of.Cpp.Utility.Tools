#include "File.hpp"
#include "Visitor.hpp"

File::File(const std::string& name_, int size_) : name(name_), size(size_) {}

File::~File() {}

std::string File::getName() {
    return name;
}

int File::getSize() {
    return size;
}

void File::accept(Visitor* v) {
    v->visit(this);
}
