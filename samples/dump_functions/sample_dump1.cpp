#include "dump_functions.hpp"

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
	int length = static_cast<int>(file.tellg());
	file.seekg(0, file.beg);

    const int BUF_SIZE = DUMP_COLS_PER_ROW;
    uint8_t buf[BUF_SIZE] = {0}; 

	while (length >= BUF_SIZE) {
		file.read((char *) buf, BUF_SIZE);
        dump(buf, BUF_SIZE, NULL);
		length -= BUF_SIZE;
	}

	file.read((char *) buf, length);
	int margin_length = 3*(DUMP_COLS_PER_ROW - length);
	if (length < DUMP_SPACE_COLS)
		margin_length += 1;
	margin_length += 2;
	std::string margin(margin_length, ' '); 
	dump_hex(buf, length, NULL);
	dump_ascii(buf, length, margin.c_str());

	printf("\n\n");

    fflush(stdout);

    return 0;
}
