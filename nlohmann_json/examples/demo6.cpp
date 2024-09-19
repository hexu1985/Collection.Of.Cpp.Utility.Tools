#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
    //从字符串文字创建对象 
    json j = " { \" happy \": true, \" pi \": 3.141} "_json;

    //或者原始字符串文字
    auto j2 = R"(
    { 
        "happy":true,
        "pi":3.141 
    } 
    )"_json;

    //显式转换为string 
    std::string s = j.dump();    // {\"happy\":true,\"pi\":3.141}
    std::cout << s << std::endl;

    //序列化与漂亮的打印
    //传入空格的数量缩进 
    std::cout << j2.dump(4) << std::endl;
    // 输出：
    //{
    //     "happy": true,
    //     "pi": 3.141
    // }

    return 0;
}
