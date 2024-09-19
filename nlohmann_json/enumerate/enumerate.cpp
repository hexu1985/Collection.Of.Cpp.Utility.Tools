#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
    std::string filename = "maildata.json";
    std::ifstream ifile(filename);
    if (!ifile) {
        std::cerr << "Warning: " << filename << " is not found." << '\n';
        return {};
    }

    nlohmann::json prop;
    ifile >> prop;
    for (auto it = prop.begin(); it != prop.end(); ++it) {
        std::cerr << "key: " << it.key() << ", value: " << it.value() << "\n";
    }

    return 0;
}
