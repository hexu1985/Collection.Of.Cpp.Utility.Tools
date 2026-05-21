#include <string>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

vector<string> split(const string& s, char c=' ') {
    vector<string> v;
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while (j != string::npos) {
        v.push_back(s.substr(i, j-i));
        if (c == ' ') {
            while (s[j+1] == ' ') ++j;
        }
        i = ++j;
        j = s.find(c, j);

        if (j == string::npos)
            v.push_back(s.substr(i, s.length( )));
    }
    return v;
}

void test(const std::string& s, char c) {
    cout << "split[" << s << "]\n";
    vector<string> v = split(s, c);

    for (size_t i = 0; i < v.size( ); ++i) {
        cout << v[i] << '\n';
    }
}

void test_1() {
    string s = "AccountName   Address1       Address2         City";
    test(s, ' ');
}

void test_2() {
    string s = "Account Name|Address 1|Address 2|City";
    test(s, '|');
}

void test_3() {
    string s = "Account Name||Address 1|Address 2|City";
    test(s, '|');
}

int main( ) {
    test_1();
    test_2();
    test_3();
}
