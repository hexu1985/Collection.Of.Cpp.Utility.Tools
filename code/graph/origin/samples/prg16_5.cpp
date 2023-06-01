#ifdef _MSC_VER
// disable warning messages that identifier was truncated
// to 'number' characters in the debug information
#pragma warning(disable:4786)
#endif	// _MSC_VER

// File: prg16_5.cpp
// the program inputs the graph specified by the file "mpath.dat".
// in a loop that executes four times, a prompt directs the user
// to enter starting and ending vertices. by calling the algorithm
// minimumPath(), the program outputs the minimum path weight and
// the corresponding path from the starting to the ending vertex

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>

#include "d_graph.h"
#include "d_util.h"

using namespace std;

int main()
{
   // vertices are characters
   graph<char> g;
	char sVertex, eVertex;

	// minimum path to the destination vertex
	list<char> path;

	ifstream graphIn;
	graphIn.open("mpath.dat");

   // input the graph
   graphIn >> g;

	for (int i = 1; i <= 4; i++)
	{
		cout << "Enter start and end vertices: ";
		cin >> sVertex >> eVertex;

		cout  << "    Minimum path from " << sVertex << " to " 
				<< eVertex << " is " << setw(3)
			   << minimumPath(g, sVertex, eVertex, path) << "   Path: ";
		writeContainer(path.begin(), path.end());
		cout << endl << endl;
	}

	return 0;
}

/*
Run:

Enter start and end vertices: F C
    Minimum path from F to C is  38   Path: F  E  D  A  B  C

Enter start and end vertices: C F
    Minimum path from C to F is  -1   Path:

Enter start and end vertices: B B
    Minimum path from B to B is   0   Path: B

Enter start and end vertices: A C
    Minimum path from A to C is  10   Path: A  B  C
*/
