#include "thread_name.hpp"

namespace current_thread {

std::string& get_thread_name_ref() {
    static thread_local std::string tls_thread_name{"unknown"};
    return tls_thread_name;
}

void set_thread_name(const std::string& thread_name) {
    get_thread_name_ref() = thread_name;
}

const std::string& get_thread_name() {
    return get_thread_name_ref();
}

}   // namespace current_thread


