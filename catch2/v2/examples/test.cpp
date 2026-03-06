// 必须定义此宏，Catch2 会提供 main 函数
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

// 待测试的简单函数
int add(int a, int b) {
    return a + b;
}

// 测试用例
TEST_CASE("Addition works", "[math]") {
    REQUIRE(add(2, 3) == 5);
    REQUIRE(add(-1, 1) == 0);
    REQUIRE(add(0, 0) == 0);
}

TEST_CASE("Another test", "[example]") {
    SECTION("some section") {
        REQUIRE(add(5, 7) == 12);
    }
}
