#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: prg16_6.cpp
// this program illustrates Prim's algorithm for a minimum spanning tree.
// it reads a graph input file whose name is entered by the user and
// calls minSpanTree() to construct the minimum spanning tree for the
// graph. output includes the total weight for the minimum spanning tree as
// well as a listing of its vertices and edges 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

#include "d_graph.h"

int main()
{
	// graph input file
	ifstream graphIn;
	// vertices are characters
	graph<char> g, minSpan;
	int weight;
	// graph file name
	string fileName;

	cout << "Graph input file: ";
	cin >> fileName;

	graphIn.open(fileName.c_str());
	if (!graphIn)
	{
		cerr << "Cannot open '" << fileName << "'" << endl;
		exit(1);
	}

   // input the graph
   graphIn >> g;

	// get minimum spanning tree and its weight
	weight = minSpanTree(g, minSpan);
	cout << "MST has weight "<< weight << endl << endl;

	// display minumum spanning tree
	cout << " --- MST Graph ---" << endl;
	cout << minSpan << endl;

	return 0;
}

/*
Run 1:

Graph input file: minspan.dat
MST has weight 14

 --- MST Graph ---
A: in-degree 0  out-degree 2
    Edges: B (2)  D (5)
B: in-degree 1  out-degree 0
    Edges:
C: in-degree 1  out-degree 0
    Edges:
D: in-degree 1  out-degree 1
    Edges: C (7)

Run 2:

Graph input file: network.dat
MST has weight 241

 --- MST Graph ---
A: in-degree 0  out-degree 1
    Edges: C (25)
B: in-degree 1  out-degree 2
    Edges: D (25)  F (23)
C: in-degree 1  out-degree 1
    Edges: B (46)
D: in-degree 1  out-degree 1
    Edges: E (35)
E: in-degree 1  out-degree 0
    Edges:
F: in-degree 1  out-degree 2
    Edges: G (55)  H (32)
G: in-degree 1  out-degree 0
    Edges:
H: in-degree 1  out-degree 0
    Edges:
*/
