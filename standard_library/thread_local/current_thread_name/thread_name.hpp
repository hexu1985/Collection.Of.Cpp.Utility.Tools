#pragma once

#include <string>

namespace current_thread {

void set_thread_name(const std::string& thread_name);

const std::string& get_thread_name();

}   // namespace current_thread

