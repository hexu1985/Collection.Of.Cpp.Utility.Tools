#include <stdio.h>
#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
    const json obj = {
            {"name", "哥伦布啊"},
            {"gold", 1234}
            };
    std::cout << obj.dump() << std::endl;

    std::cout << "========= array ==========" << std::endl;
    json array = json::array();
    array.push_back({{"name", "hexu"}, {"weight", 75}});
    array.push_back({{"name", "xxx"}, {"weight", 77}});
    array.push_back({{"name", "yyy"}, {"weight", 66}});
    std::cout << array.dump() << std::endl;

	return 0;
}
