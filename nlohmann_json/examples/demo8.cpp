#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
	//明确解析
	auto j3 = json::parse(" { \"happy\": true, \"pi\":3.141 } ");
	std::cout << j3 << std::endl;
	std::cout << j3["happy"] << std::endl;
	std::cout << j3["pi"] << std::endl;
	bool is_happy = j3["happy"];
	double pi = j3["pi"];
	std::cout << is_happy << std::endl;
	std::cout << pi << std::endl;
	return 0;
}

