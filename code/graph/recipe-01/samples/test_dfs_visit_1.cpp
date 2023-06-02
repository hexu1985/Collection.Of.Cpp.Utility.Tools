// File: sample16_4_1.cpp
// the program inputs the graph specified by the file "dfs_visit.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "graph.hpp"
#include "graph_io.hpp"
#include "dfs_visit.hpp"

using namespace std;

int main()
{
	Graph<char> g;

	ifstream graphIn;
	graphIn.open("dfs_visit.dat");

	// input the graph and then prompt for input of
	// source vertices
	graphIn >> g;

    list<char> finishOrder;
    dfs_visit(g, 'B', finishOrder, false);
    cout  << "Searching grap from vertex " << 'B' << endl;
    for (auto v : finishOrder) { cout << v << "  "; }
    // finishOrder: B D E F C
    cout << endl;

	return 0;
}

/*
Run:

Searching grap from vertex B
B  D  E  F  C
*/

