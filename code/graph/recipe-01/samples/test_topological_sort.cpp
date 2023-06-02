// File: prg16_3.cpp
// the program inputs a directed acyclic graph that describes course
// prerequisites in a religious studies departmen. it performs
// a topological sort and outputs the result as a possible schedule
// of courses

#include <iostream>
#include <string>
#include <list>

#include "graph.hpp"
#include "graph_io.hpp"
#include "topological_sort.hpp"

using namespace std;

int main()
{
	// graph specifying the courses and prerequisite edges
	Graph<string> g;

	// a list holding the topological order of courses
	list<string> tlist;

	// input file holding the vertex strings and edges
	ifstream graphIn;

	// open the file and input the graph
	graphIn.open("courses.dat");
	graphIn >> g;

	// execute a topological sort; store results in list
	topological_sort(g,tlist);

	// output the list of possible courses 
	cout << "Possible schedule of courses"
		  << endl << "    ";
    for (auto v : tlist) { cout << v << "  "; }
	cout << endl;

	return 0;
}

/*
Run:

Possible schedule of courses
    R51  R53  R173  R151  R37  R137  R63  R263
*/
