#ifndef RC_WIDGET_INC
#define RC_WIDGET_INC

#include "RCObject.hpp"
#include "RCPtr.hpp"
#include "widget.hpp"

using namespace mini_utils;

class RCWidget {
public:
    RCWidget(const std::string &someThing): widget_(new WidgetHolder(someThing))
    {
    }

    void setValue(const std::string &value)
    {
        widget_->setValue(value);
    }

    void doThis() { widget_->doThis(); }
    const std::string &showThat() const { return widget_->showThat(); }

private:
    class WidgetHolder: public Widget, public RCObject {
    public:
        WidgetHolder(const std::string &someThing): Widget(someThing) {}
    };

    RCPtr<WidgetHolder> widget_;
};

#endif
