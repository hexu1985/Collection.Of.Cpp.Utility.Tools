#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {

	std::map<std::string, int> c_map { {"one", 1}, {"two", 2}, {"three", 3} };
	json j_map(c_map);
	// {"one": 1, "three": 3, "two": 2 }
	std::cout << j_map << std::endl;
	
	std::unordered_map<const char*, double> c_umap { {"one", 1.2}, {"two", 2.3}, {"three", 3.4} };
	json j_umap(c_umap);
	// {"one": 1.2, "two": 2.3, "three": 3.4}
	std::cout << j_umap << std::endl;
	
	std::multimap<std::string, bool> c_mmap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
	json j_mmap(c_mmap); // only one entry for key "three" is used
	// maybe {"one": true, "two": true, "three": true}
	std::cout << j_mmap << std::endl;
	
	std::unordered_multimap<std::string, bool> c_ummap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
	json j_ummap(c_ummap); // only one entry for key "three" is used
	// maybe {"one": true, "two": true, "three": true}
	std::cout << j_ummap << std::endl;

	return 0;
}

