#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {

	{
		json array = {
			"yan",12,"ze",13
		};
		std::cout << array[0] << std::endl;
	}

	{
		json array = {
			{"yan",12},
			"ze",13
		};
		std::cout << array[0][0] << std::endl;
	}

	{
		json array = {
			{{"yan",12}},
			"ze",13
		};
		std::cout << array[0]["yan"] << std::endl;
	}

	{
		json array = {
			{"yan",12},
			{"ze",13}
		};
		std::cout << array["yan"] << std::endl;
	}

	{
		json array = {
			{
				{"yan",12}
			},
			{
				{"ze",13}
			}
		};
		std::cout << array[0]["yan"] << std::endl;
	}

	return 0;
}

