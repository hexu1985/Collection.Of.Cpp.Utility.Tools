#pragma once

#include "File.hpp"
#include "Directory.hpp"

class Visitor {
public:
    virtual ~Visitor();

    virtual void visit(File* file) = 0;
    virtual void visit(Directory* directory) = 0;
};
