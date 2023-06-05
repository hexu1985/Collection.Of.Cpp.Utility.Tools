#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: sample16_5.cpp
// the program inputs the graph specified by the file "dfs.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "d_graph.h"
#include "d_util.h"

using namespace std;

int main()
{
	graph<char> g;
    list<char> dfsList;

	ifstream graphIn;
	graphIn.open("dfs.dat");

	// input the graph
	graphIn >> g;

    dfs(g, dfsList);

	cout  << "dfsList:  ";
	writeContainer(dfsList.begin(), dfsList.end());
	cout << endl;

	return 0;
}

/*
Run:

dfsList:  E  F  G  D  A  B  C

*/
