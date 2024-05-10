#pragma once

#include "Entry.hpp"
#include <string>

class File : public Entry {
private:
    std::string name;
    int size;

public:
    File(const std::string& name, int size);
    ~File() override;

    std::string getName() override;     // 获取名字
    int getSize() override;             // 获取大小

    void accept(Visitor* v) override;   // 接受访问者的访问
};
