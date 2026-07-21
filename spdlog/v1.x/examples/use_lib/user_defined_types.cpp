#include "spdlog/spdlog.h"

struct my_type {
    my_type(int i_): i(i_) {}

    int i;
};

template<>
struct fmt::formatter<my_type> : fmt::formatter<std::string>
{
    auto format(my_type my, format_context &ctx) const -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "[my_type i={}]", my.i);
    }
};

void user_defined_example()
{
    spdlog::info("user defined type: {}", my_type(14));
}

int main()
{
    user_defined_example();
}
