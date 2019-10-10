#ifndef RC_WIDGET_INC
#define RC_WIDGET_INC

#include "RCIPtr.hpp"
#include "widget.hpp"

using namespace mini_utils;

class RCWidget {
public:
    RCWidget(const std::string &someThing): widget_(new Widget(someThing))
    {
    }

    void setValue(const std::string &value)
    {
        widget_->setValue(value);
    }

    void doThis() { widget_->doThis(); }
    const std::string &showThat() const { return widget_->showThat(); }

private:
    RCIPtr<Widget> widget_;
};

#endif
