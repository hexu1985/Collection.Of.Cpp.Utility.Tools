#pragma once

void error_file_line(const char* file, unsigned int line, const char* format, ...);

#define error(format, ...)  error_file_line(__FILE__, __LINE__, format, __VA_ARGS__)

