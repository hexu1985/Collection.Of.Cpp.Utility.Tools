#include <string>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

vector<string> split(const string& s, char c) {
    vector<string> v;
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while (j != string::npos) {
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == string::npos)
            v.push_back(s.substr(i, s.length( )));
    }
    return v;
}

int main( ) {
    string s = "Account Name|Address 1|Address 2|City";

    vector<string> v = split(s, '|');

    for (size_t i = 0; i < v.size( ); ++i) {
        cout << v[i] << '\n';
    }
}
