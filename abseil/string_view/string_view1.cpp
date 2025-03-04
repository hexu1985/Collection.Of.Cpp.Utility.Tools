#include <iostream>
#include <string>
#include <absl/strings/string_view.h>

int main() {
  std::string s = "Tired like a dog";
  absl::string_view sv(s);  // 用 std::string 初始化

  const char *cs = "Hello world"; 
  absl::string_view sv1(cs);  // 用字符串指针初始化
  absl::string_view sv2(cs + 6, 5); // 用字符串区间初始化，输出 "world"
  absl::string_view sv3(cs + 6, 8); // string_view 会自动判断字符串结尾，不会溢出，同样输出 "world"

  absl::string_view sv4;
  sv4 = sv3;   // string_view 之间可以直接赋值

  std::cout << sv << std::endl;
  std::cout << sv1 << std::endl; 
  std::cout << sv2 << std::endl;
  std::cout << sv3 << std::endl;
  std::cout << sv4 << std::endl;
  std::cout << sv4[2] << std::endl;  // 可以用方括号获取string_view 中的字符
  // sv4[2]  = 'i';  // ERROR：不允许通过方括号更改底层字符，因为string_view 不持有底层字符
  return 0;
}
