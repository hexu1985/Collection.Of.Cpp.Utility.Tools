#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {

	/// strings
	std::string s1 = "Hello, world!";
	json js = s1;
	std::string s2 = js;

	// Booleans
	bool b1 = true;
	json jb = b1;
	bool b2 = jb;

	// numbers
	int i = 42;
	json jn = i;
	double f = jn;

	// etc.

	std::string vs = js.get<std::string>();
	bool vb = jb.get<bool>();
	int vi = jn.get<int>();
	// etc.


	return 0;
}

