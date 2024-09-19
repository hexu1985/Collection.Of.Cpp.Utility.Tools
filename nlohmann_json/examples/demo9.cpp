#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {

	{
		std::vector<std::uint8_t> v = {'t', 'r', 'u', 'e'};
		json j = json::parse(v.begin(), v.end());
	}

	{
		std::vector<std::uint8_t> v = {'t', 'r', 'u', 'e'};
		json j = json::parse(v);
	}

	return 0;
}

