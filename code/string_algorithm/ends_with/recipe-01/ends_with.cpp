#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

bool ends_with(const std::string& input, const std::string& test) {
    return input.rfind(test) == (input.length() - test.length());
}

int main(int argc, char *argv[])
{
    string s = "abcd.part";
    string suffix = ".part";

    cout << s << " is endswith " << suffix << " ? "
        << boolalpha << ends_with(s, suffix) << endl;

    suffix = ".idx";

    cout << s << " is endswith " << suffix << " ? "
        << boolalpha << ends_with(s, suffix) << endl;

    return 0;
}
