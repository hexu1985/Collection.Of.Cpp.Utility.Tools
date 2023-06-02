#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: sample16_4_1.cpp
// the program inputs the graph specified by the file "dfs_visit.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "d_graph.h"
#include "d_util.h"

using namespace std;

int main()
{
	graph<char> g;

	ifstream graphIn;
	graphIn.open("dfs_visit.dat");

	// input the graph and then prompt for input of
	// source vertices
	graphIn >> g;

    list<char> finishOrder;
    dfsVisit(g, 'B', finishOrder, false);
    cout  << "Searching grap from vertex " << 'B' << endl;
    writeContainer(finishOrder.begin(), finishOrder.end());
    // finishOrder: B D E F C
    cout << endl;

	return 0;
}

/*
Run:

Searching grap from vertex B
B  D  E  F  C
*/

