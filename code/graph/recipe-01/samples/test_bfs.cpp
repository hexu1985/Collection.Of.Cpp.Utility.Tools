// File: sample16_3.cpp
// the program inputs the graph specified by the file "bfs.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "graph.hpp"
#include "graph_io.hpp"
#include "bfs.hpp"

using namespace std;

int main()
{
	Graph<char> g;

	char startVertex;
	set<char> visitSet;

	ifstream graphIn;
	graphIn.open("bfs.dat");

	// input the graph and then prompt for input of
	// source vertices
	graphIn >> g;

	cout << "Enter start vertex: ";
	cin >> startVertex;

    visitSet = bfs(g, startVertex);

	cout  << "BFS visitSet from " << startVertex << ":  ";
    for (auto v : visitSet) { cout << v << "  "; }
	cout << endl;

	return 0;
}

/*
Run 1:

Enter start vertex: A
BFS visitSet from A:  A  B  C  D  E  F  G

Run 2:

Enter start vertex: D
BFS visitSet from D:  D  E  F  G

Run 3:

Enter start vertex: E
BFS visitSet from E:  E
*/
