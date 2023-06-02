#pragma once

#include <string>
#include <stdexcept>

// error in graph class
class GraphError: public std::runtime_error
{
public:
	GraphError(const std::string& msg = ""):
		std::runtime_error(msg)
	{}
};

