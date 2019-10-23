#include "DumpHelper.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace mini_utils;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " file.\n";
        return -1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cout << "open " << argv[1] << " fail\n";
        return -2;
    }

	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

    const int BUF_SIZE = 1024;
    uint8_t buf[BUF_SIZE] = {0}; 

	DumpHelper helper;

	while (length >= BUF_SIZE) {
		file.read((char *) buf, BUF_SIZE);
        helper.dump(buf, BUF_SIZE);
		length -= BUF_SIZE;
	}
	file.read((char *) buf, length);
	helper.dump(buf, length);

	std::cout << std::endl;

    return 0;
}
