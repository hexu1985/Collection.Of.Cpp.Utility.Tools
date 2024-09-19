#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
	// store a string in a JSON value
	json j_string = "this is a string";

	// retrieve the string value
	auto cpp_string = j_string.get<std::string>();
	// retrieve the string value (alternative when an variable already exists)
	std::string cpp_string2;
	j_string.get_to(cpp_string2);

	// retrieve the serialized value (explicit JSON serialization)
	std::string serialized_string = j_string.dump();

	// output of original string
	std::cout << cpp_string << " == " << cpp_string2 << " == " << j_string.get<std::string>() << '\n';
	// output of serialized value
	std::cout << j_string << " == " << serialized_string << std::endl;
    return 0;
}
