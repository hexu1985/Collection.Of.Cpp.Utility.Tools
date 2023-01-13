// C++ program to remove white spaces
// from string using the function
// boost::trim function
#include <iostream>

void trim_left(std::string& s) {
    const static std::string whitespaces (" \t\f\v\n\r"); 

    if (s.empty())
        return;
    
    size_t found = s.find_first_not_of(whitespaces);
    if (found != std::string::npos)
        s.erase(0, found);
    else
        s.clear();            // s is all whitespace
}

void trim_right(std::string& s) {
    const static std::string whitespaces (" \t\f\v\n\r"); 

    if (s.empty())
        return;
    
    size_t found = s.find_last_not_of(whitespaces);
    if (found != std::string::npos)
        s.erase(found+1);
    else
        s.clear();            // s is all whitespace
}

void trim(std::string& s) {
    trim_left(s);
    trim_right(s);
}

using namespace std;
  
// Driver Code
int main()
{
    // Given Input
    string s1 = "    geeks_for_geeks    ";
    string s2 = "    geeks_for_geeks    ";
    string s3 = "    geeks_for_geeks    ";
  
    // Apply Left Trim on string, s1
    cout << "The original string is:\""
         << s1 << "\" \n";
    trim_left(s1);
    cout << "Applied left trim:\""
         << s1 << "\" \n\n";
  
    // Apply Right Trim on string, s2
    cout << "The original string is:\""
         << s2 << "\" \n";
    trim_right(s2);
    cout << "Applied right trim:\""
         << s2 << "\" \n\n";
  
    // Apply Trim on string, s3
    cout << "The original string is:\""
         << s3 << "\" \n";
    trim(s3);
    cout << "Applied trim:\"" << s3
         << "\" \n";
  
    return 0;
}
