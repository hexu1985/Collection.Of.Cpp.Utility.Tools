#include "widget.hpp"
#include <iostream>

Widget::Widget(const std::string &someThing): someThing(someThing) 
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "(" << someThing << ")" << std::endl;
}

Widget::Widget(const Widget &rhs): someThing(rhs.someThing)
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "(" << (void *) &rhs << ")" << std::endl;
}

Widget::~Widget()
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "()" << std::endl;
}

Widget &Widget::operator =(const Widget &rhs)
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "(" << (void *) &rhs << ")" << std::endl;
    someThing = rhs.someThing;
    return *this;
}

void Widget::setValue(const std::string &value)
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "(" << value << ")" << std::endl;
    someThing = value;
}

void Widget::doThis()
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "()" << std::endl;
}

const std::string &Widget::showThat() const
{
    std::cout << "[" << someThing << "]: " << (void *) this << "->" << __func__ << "()" << std::endl;
    return someThing;
}
