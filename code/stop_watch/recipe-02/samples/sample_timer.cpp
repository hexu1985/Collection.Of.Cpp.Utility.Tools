// File: prg3_5.cpp
// the program evaluates Fibonacci number 45 using
// an iterative and a recursive algorithm. the iterative
// algorithm executes in a fraction of a second. the program
// uses a timer object to output the computation time
// of the very inefficient recursive algorithm (exponential
// running time)

#include <iostream>

#include "timer.h"

using namespace std;

// prototypes for the recursive and iterative versions
int fib(int n);
int fibiter(int n);

int main()
{
    timer t;			// time recursive version
    int fib_45;

    // evaluate fib() using iteration and recursion
    cout << "Value of fib(45) by iteration is ";
    cout << fibiter(45) << endl;

    cout << "Value of fib(45) by recursion is ";
    // start/stop timer object for recursive process
    t.start();
    fib_45 = fib(45);
    t.stop();

    cout << fib_45 << endl;
    cout << "   Time required by recursive version is " << t.time() 
        << " sec" <<endl;

    return 0;
}

int fib(int n)
{
    if (n <= 1)								// stopping conditions
        return n;
    else
        return fib(n-1) + fib(n-2);	// recursive step
}

int fibiter(int n)
{
    // integers to store previous two Fibonacci value 
    int oneback = 1, twoback = 0, current;
    int i;

    // return is immediate for first two numbers
    if (n <= 1)
        return n;
    else
        // compute successive terms beginning at 3
        for (i = 2; i <= n; i++)
        {
            current = oneback + twoback;
            twoback = oneback;	// update for next calculation
            oneback = current;
        }

    return current;			
}

/*
Run:

Value of fib(45) by iteration is 1134903170
Value of fib(40) by recursion is 1134903170
   Time required by recursive version is 72.584 sec
*/
