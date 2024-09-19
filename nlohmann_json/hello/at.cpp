#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() {
	json j;			// 创建 json 对象
    std::ifstream jfile("test.json");
	jfile >> j;		// 以文件流形式读取 json 文件
	float pi = j.at("pi");
    std::cout << "pi: " << pi << std::endl;
	bool happy = j.at("happy");
    std::cout << "happy: " << happy << std::endl;
    std::string nosuch;
    try {
        nosuch = j.at("nosuch");
    } catch(nlohmann::detail::out_of_range) {
        nosuch = "None";
    }
    std::cout << "nosuch: " << nosuch << std::endl;
	return 0;
}
