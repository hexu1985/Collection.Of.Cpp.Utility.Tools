#include <fstream>
#include <iostream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " file\n";
        return -1;
    }

    std::ifstream ifile(argv[1]);
    if (!ifile) {
        std::cerr << "file " << argv[1] << ", not exists!\n";
        return -2;
    }

	json js;
	ifile >> js;

    std::cout << js["carriorName"].get<std::string>() << std::endl;
    std::cout << std::setw(4) << js << std::endl;

	return 0;
}

