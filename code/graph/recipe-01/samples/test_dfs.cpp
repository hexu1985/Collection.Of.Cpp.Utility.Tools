// File: sample16_5.cpp
// the program inputs the graph specified by the file "dfs.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "graph.hpp"
#include "graph_io.hpp"
#include "dfs.hpp"

using namespace std;

int main()
{
    Graph<char> g;
    list<char> dfsList;

    ifstream graphIn;
    graphIn.open("dfs.dat");

    // input the graph
    graphIn >> g;

    dfs(g, dfsList);

    cout  << "dfsList:  ";
    for (auto v : dfsList) { cout << v << "  "; }
    cout << endl;

    return 0;
}

/*
Run:

dfsList:  E  F  G  D  A  B  C

*/
