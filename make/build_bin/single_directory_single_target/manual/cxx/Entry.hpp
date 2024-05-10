#pragma once

#include "Element.hpp"
#include <string>
#include <iosfwd>

class Entry : public Element {
public:
    virtual ~Entry();

    virtual std::string getName() = 0;      // 获取名字
    virtual int getSize() = 0;              // 获取大小
    std::string toString();                 // 显示字符串
};

std::ostream& operator<<(std::ostream& out, Entry& entry);
