#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: prg16_1.cpp
// the program inputs the graph whose vertices are characters from the
// file "graphIO.dat" and calls the member function getNeighbors() to
// find set of neighbors for 'A'. using the function writeContainer()
// from "d_util.h", the program outputs the adjacent vertices. a call
// to the overloaded stream operator displays the properties of the
// graph

#include <iostream>
#include <fstream>
#include <set>

#include "d_graph.h"		// the graph class
#include "d_util.h"		// function writeContainer()

using namespace std;

int main()
{
	// graph with vertices of type char
	graph<char> demoGraph;

	// edge set
	set<char> edgeSet;

	// input stream for graph data
	ifstream graphIn;

	// open stream and input the graph
	graphIn.open("graphIO.dat");
	graphIn >> demoGraph;

	// get set of neighbors of A and output with writeContainer
	edgeSet = demoGraph.getNeighbors('A');

	cout << "Neighbors of A are ";
	writeContainer(edgeSet.begin(), edgeSet.end());
	cout << endl << endl;
	
	// output the graph and its properties
	cout << demoGraph << endl;

	return 0;
}

/*
Input file: graphIO.dat
5
A B C D E
8
A  B  4
A  C  7
A  D  6
B  A  2
C  B  3
C  E  2
D  E  4
E  C  1

Run:

Neighbors of A are B  C  D

A: in-degree 1  out-degree 3
    Edges: B (4)  C (7)  D (6)
B: in-degree 2  out-degree 1
    Edges: A (2)
C: in-degree 2  out-degree 2
    Edges: B (3)  E (2)
D: in-degree 1  out-degree 1
    Edges: E (4)
E: in-degree 2  out-degree 1
    Edges: C (1)
*/
