#pragma once

#include "Entry.hpp"
#include <string>
#include <vector>

class Directory : public Entry {
private:
    std::string name;           // 文件夹名字
    std::vector<Entry*> dir;    // 目录条目集合

public:
    Directory(const std::string& name); // 构造函数
    ~Directory() override;

    std::string getName() override;     // 获取名字
    int getSize() override;             // 获取大小

    Entry* add(Entry* entry);           // 增加目录条目

    void accept(Visitor* v) override;   // 接受访问者的访问

    using iterator = std::vector<Entry*>::iterator;
    iterator begin() { return dir.begin(); }
    iterator end() { return dir.end(); }
};


