#include <string>
#include <iostream>
#include <iomanip>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

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
