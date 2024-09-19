#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {

    // Using (raw) string literals and json::parse
    json ex1 = json::parse(R"(
  {
    "pi": 3.141,
    "happy": true
  }
)");

    // Using user-defined (raw) string literals
    using namespace nlohmann::literals;
    json ex2 = R"(
  {
    "pi": 3.141,
    "happy": true
  }
)"_json;

    // Using initializer lists
    json ex3 = {
        {"happy", true},
        {"pi", 3.141},
    };

    std::cout << ex1 << std::endl;
    std::cout << ex2 << std::endl;
    std::cout << ex3 << std::endl;


    return 0;
}
