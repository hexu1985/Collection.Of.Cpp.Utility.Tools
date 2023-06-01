#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: prg16_3.cpp
// the program inputs a directed acyclic graph that describes course
// prerequisites in a religious studies departmen. it performs
// a topological sort and outputs the result as a possible schedule
// of courses

#include <iostream>
#include <string>
#include <list>

#include "d_graph.h"
#include "d_util.h"

using namespace std;

int main()
{
	// graph specifying the courses and prerequisite edges
	graph<string> g;

	// a list holding the topological order of courses
	list<string> tlist;

	// input file holding the vertex strings and edges
	ifstream graphIn;

	// open the file and input the graph
	graphIn.open("courses.dat");
	graphIn >> g;

	// execute a topological sort; store results in list
	topologicalSort(g,tlist);

	// output the list of possible courses 
	cout << "Possible schedule of courses"
		  << endl << "    ";
	writeContainer(tlist.begin(), tlist.end());
	cout << endl;

	return 0;
}

/*
Run:

Possible schedule of courses
    R51  R53  R173  R151  R37  R137  R63  R263
*/
