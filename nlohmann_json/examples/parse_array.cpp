#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
    std::string info = R"({"id":"1111111","cards":[0,1,2,3,4,5,6,7,8,9],"jiaoDiZhu":""})";
    auto doc = json::parse(info);

    auto &dataArray = doc["cards"];

    if (dataArray.is_array())
    {
        for (auto &object : dataArray)
        {
//			std::cout << object << std::endl;
			std::cout << object.get<int>() << std::endl;
        }
    }

    return 0;
}

