#ifndef WIDGET_INC
#define WIDGET_INC

#include <string>

class Widget {
public:
    Widget(const std::string &someThing);
    Widget(const Widget &rhs);
    ~Widget();

    Widget &operator =(const Widget &rhs);

    void setValue(const std::string &value);
    void doThis();
    const std::string &showThat() const;

private:
    std::string someThing;
};

#endif
