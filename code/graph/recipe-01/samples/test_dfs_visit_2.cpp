// File: sample16_4_2.cpp
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

    try 
    {
        list<char> finishOrder;
        dfs_visit(g, 'E', finishOrder, true);
    }
    catch (GraphError& ge)
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

