#include<iostream>
using namespace std;
int main() {
    int a = 5;
    cout << "a = " << a << endl;
    cout << "&a = " << &a << endl;
    // Placement new changes the value of X to 100
    int *m = new (&a) int(10);
    cout << "\nAfter using placement new:" << endl;
    cout << "a = " << a << endl;
    cout << "m = " << m << endl;
    cout << "&a = " << &a << endl;
    return 0;
}
