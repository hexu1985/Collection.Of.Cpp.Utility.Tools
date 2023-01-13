// C++ program to remove white spaces
// from string using the function
// boost::trim function
#include <boost/algorithm/string.hpp>
#include <iostream>
using namespace boost::algorithm;
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
