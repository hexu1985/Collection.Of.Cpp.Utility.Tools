#pragma once

#include "Visitor.hpp"

class ListVisitor : public Visitor {
private:
    std::string currentdir;     // 当前访问的文件夹的名字

public:
    ListVisitor();
    ~ListVisitor() override;

    void visit(File* file) override;
    void visit(Directory* directory) override;
};
