#include "widget.hpp"
#include <iostream>

Widget::Widget(const std::string &someThing): someThing_(someThing) 
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "(" << someThing << ")" << std::endl;
}

Widget::Widget(const Widget &rhs): someThing_(rhs.someThing_)
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "(" << (void *) &rhs << ")" << std::endl;
}

Widget::~Widget()
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "()" << std::endl;
}

Widget &Widget::operator =(const Widget &rhs)
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "(" << (void *) &rhs << ")" << std::endl;
    someThing_ = rhs.someThing_;
    return *this;
}

void Widget::setValue(const std::string &value)
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "(" << value << ")" << std::endl;
    someThing_ = value;
}

void Widget::doThis()
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "()" << std::endl;
}

const std::string &Widget::showThat() const
{
    std::cout << "[" << someThing_ << "]: " << (void *) this << "->" << __func__ << "()" << std::endl;
    return someThing_;
}
