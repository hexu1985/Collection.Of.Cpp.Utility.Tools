#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
    // create an array using push_back
    json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);

    // iterate the array
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << *it << '\n';
    }

    // range-based for
    for (auto element : j) {
        std::cout << element << '\n';
    }

    // getter/setter
    const std::string tmp = j[0];
    j[1] = 42;
    bool foo = j.at(2);

    // other stuff
    j.size();     // 3 entries
    j.empty();    // false
    j.type();     // json::value_t::array
    j.clear();    // the array is empty again

    // convenience type checkers
    j.is_null();
    j.is_boolean();
    j.is_number();
    j.is_object();
    j.is_array();
    j.is_string();

    // comparison
    std::cout << (j == "[\"foo\", 1, true]"_json) << std::endl;  // true

    // create an object
    json o;
    o["foo"] = 23;
    o["bar"] = false;
    o["baz"] = 3.141;

    // special iterator member functions for objects
    for (json::iterator it = o.begin(); it != o.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << "\n";
    }

    // find an entry
    if (o.find("foo") != o.end()) {
        // there is an entry with key "foo"
        std::cout << *o.find("foo") << std::endl;
    }

    // or simpler using count()
    int foo_present = o.count("foo"); // 1
    int fob_present = o.count("fob"); // 0
    std::cout << foo_present << std::endl;
    std::cout << fob_present << std::endl;
    // delete an entry
    o.erase("foo");
    std::cout << (o.find("foo") == o.end()) << std::endl;
    return 0;
}

