#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: prg16_2.cpp
// the program inputs a directed acyclic graph (DAG) from the file
// "dag.dat" and calls the graph algorithm acyclic() to verify that
// it has no cycles. adding an edge (E,A) creates a cycle in the
// graph. a call to acyclic() verifies the presence of the cycle

#include <iostream>
#include <fstream>
#include <string>

#include "d_graph.h"

using namespace std;

int main()
{
	graph<char> g;
	// the file that defines the graph
	ifstream graphIn;

	graphIn.open("dag.dat");

	// read the graph
	graphIn >> g;

	// determine if the graph is acyclic
	if (acyclic(g))
		cout << "Original graph is acyclic" << endl;
	else
		cout << "Original graph is not acyclic" << endl;

	// add edge (E,A) to create a cycle
	cout << "   Adding edge (E,A): ";
	g.insertEdge('E', 'A', 1);

	// retest the graph to see if it is acyclic
	if (acyclic(g))
		cout << "New graph is acyclic" << endl;
	else
		cout << "New graph is not acyclic" << endl;

	return 0;
}

/*
Run:

Original graph is acyclic
   Adding edge (E,A): New graph is not acyclic
*/
