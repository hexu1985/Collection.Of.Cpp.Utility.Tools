#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: sample16_4_2.cpp
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

    try 
    {
        list<char> finishOrder;
        dfsVisit(g, 'E', finishOrder, true);
    }
    catch (graphError& ge)
    {
        cout << "Searching graph from vertex " << 'E'
            << endl;
        cout << ge.what() << endl;
    }

	return 0;
}

/*
Run:

Searching graph from vertex E
graph dfsVisit(): graph has a cycle
*/

