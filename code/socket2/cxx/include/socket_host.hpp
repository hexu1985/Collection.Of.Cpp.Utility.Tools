#pragma once

#include <unistd.h>
#include <string>

namespace Socket {

namespace host {

// 对应 gethostname(2)，返回本机主机名
inline std::string local_hostname() {
    char buf[256];
    if (::gethostname(buf, sizeof(buf)) != 0) {
        return "";
    }
    buf[sizeof(buf) - 1] = '\0';  // 防御性截断
    return buf;
}

}   // namespace host

}   // namespace Socket
