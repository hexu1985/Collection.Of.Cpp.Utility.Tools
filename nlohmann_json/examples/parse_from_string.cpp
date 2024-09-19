#include <iostream>
#include <string>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
    std::string str = R"({"carriorName":"日本电信","deviceName":"iPhone5,2","osName":"android","osVersion":"8.1","appId":"com.mmcshadow.doudizhu","versionName":"1.0","versionCode":"1.0","deviceId":"11111111"})";

    auto json = json::parse(str);

//    std::cout << json["carriorName"] << std::endl;
    std::cout << json["carriorName"].get<std::string>() << std::endl;
    std::cout << std::setw(4) << json << std::endl;

	return 0;
}
