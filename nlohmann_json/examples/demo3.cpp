#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
    // a way to express the empty array []
    json empty_array_explicit = json::array();

    // ways to express the empty object {}
    json empty_object_implicit = json({});
    json empty_object_explicit = json::object();

    // a way to express an _array_ of key/value pairs [["currency", "USD"], ["value", 42.99]]
    json array_not_object = { json::array({"currency", "USD"}), json::array({"value", 42.99}) };
    for (auto object : array_not_object) {
        std::cout << object << std::endl;
    }
    return 0;
}

