#pragma once

#include <string>
#include <stdexcept>

// error in graph class
class graphError: public std::runtime_error
{
public:
	graphError(const std::string& msg = ""):
		std::runtime_error(msg)
	{}
};

