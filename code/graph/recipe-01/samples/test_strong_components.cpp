// File: sample16_6.cpp
// the program inputs the graph specified by the file "strong_components.dat"

#include <iostream>
#include <fstream>
#include <list>

#include "graph.hpp"
#include "graph_io.hpp"
#include "strong_components.hpp"

using namespace std;

int main()
{
    Graph<char> g;
    vector<set<char>> vectSet;

    ifstream graphIn;
    graphIn.open("strong_components.dat");

    // input the graph 
    graphIn >> g;

    strong_components(g, vectSet);

    for (size_t i = 0; i < vectSet.size(); i++)
    {
        cout << "Strong Component " << i+1 << ":  ";
        for (auto v : vectSet[i]) { cout << v << "  "; }
        cout << endl;
    }

    return 0;
}

/*
Run:
Strong Component 1:  A  B  C
Strong Component 2:  E
Strong Component 3:  D  F  G
*/

