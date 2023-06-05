#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: sample16_6.cpp
// the program inputs the graph specified by the file "strong_components.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "d_graph.h"
#include "d_util.h"

using namespace std;

int main()
{
	graph<char> g;
    vector<set<char>> vectSet;

	ifstream graphIn;
	graphIn.open("strong_components.dat");

	// input the graph 
	graphIn >> g;

    strongComponents(g, vectSet);

    for (int i = 0; i < vectSet.size(); i++)
    {
        cout << "Strong Component " << i+1 << ":  ";
        writeContainer(vectSet[i].begin(), vectSet[i].end());
        cout << endl;
    }

	return 0;
}

/*
Run:
Strong Component 1:  A  B  C
Strong Component 2:  E
Strong Component 3:  D  F  G
*/

