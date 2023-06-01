#ifndef GRAPH_CLASS
#define GRAPH_CLASS

#include <iostream>
#include <fstream>

#include <set>				// set class
#include <map>				// ist classmap class
#include <vector>			// vector class
#include <list>			// list class
#include <stack>			// stack class
#include <queue>			// queue class
#include <functional>	// less<T>

#include "d_except.h"	// exception classes
#include "d_pqueue.h"	// miniPQ class

// largest positive integer on the machine
const int INFINITY = (int)((unsigned int)~0 >> 1);

using namespace std;

class neighbor
{
	public:
		// index of the destination vertex in the vector vInfo of vertex
		// properties
		int dest;
		// weight of this edge
		int weight;

		// constructor
		neighbor(int d=0, int c=0): dest(d), weight(c)
		{}

		// operators for the neighbor class that compare the
		// destination vertices
		friend bool operator< (const neighbor& lhs, const neighbor& rhs)
		{
			return lhs.dest < rhs.dest;
		}

		friend bool operator== (const neighbor& lhs, const neighbor& rhs)
		{
			return lhs.dest == rhs.dest;
		}
};

// maintains vertex properties, including its set of
// neighbors
template <typename T>
class vertexInfo
{
	public:
		// used by graph algorithms
		enum vertexColor { WHITE, GRAY, BLACK };

		// iterator pointing at a pair<T,int> object in the vertex map
		typename map<T,int>::iterator vtxMapLoc;

		// set of adjacent (neighbor) objects for the current vertex
		set<neighbor> edges;

		/// maintains the in-degree of the vertex
		int inDegree;

		// indicates whether the object currently represents a vertex
		bool occupied;

		// indicate if a vertex is marked in an algorithm that traverses
		// the vertices of a graph
		vertexColor color;

		// available to algorithms for storing relevant data values
		int dataValue;

		// available to graph algorithms; holds parent which is
		// a vertex that has an edge terminating in the current vertex
		int parent;

		// default constructor
		vertexInfo(): inDegree(0), occupied(true)
		{}

		// constructor with iterator pointing to the vertex in the map
		vertexInfo(typename map<T,int>::iterator iter):
				vtxMapLoc(iter), inDegree(0), occupied(true)
		{}
};

// priority queue data used by minimumPath() and minSpanningTree() algorithms
class minInfo
{
	public:
		int endV;
		int pathWeight;

		friend bool operator< (minInfo lhs, minInfo rhs)
		{ return lhs.pathWeight < rhs.pathWeight; }
};

template <typename T>
class graph
{
   public:
      class const_iterator: public map< T, int >::const_iterator
      {
         public:
            const_iterator()
            {}

				// converts a map iterator to a graph iterator
            const_iterator(typename std::map<T,int>::const_iterator i)
            {
					*((typename map< T, int >::const_iterator *)this) = i;
				}

				// return the vertex pointed to by the iterator
            const T& operator* () const
            {
                typename std::map<T,int>::const_iterator p = *this;

               return (*p).first;
            }
      };

		typedef const_iterator iterator;

		graph();
			// constructor. initialize numVertices and numEdges to 0

		graph(const graph<T>& g);
			// copy constructor

		graph<T>& operator= (const graph<T>& rhs);
			// overloaded assignment operator

		int numberOfVertices() const;
			// return the number of vertices in the graph

		int numberOfEdges() const;
			// return the number of edges in the graph

      bool empty() const;
			// is the graph empty?

		int getWeight(const T& v1, const T& v2) const;
			// return the weight of the edge (v1, v2). if the edge.
			// does not exist, return -1
			// Precondition: v1 and v2 are vertices in the graph. if not
			// the function throws the graphError exception

		void setWeight(const T& v1, const T& v2, int w);
			// update the weight of edge (v1, v2).
			// Precondition: v1 and v2 are vertices in the graph. if not,
			// the function throws the graphError exception
			// Postcondition: the weight of vertex (v1,v2) is w

		int inDegree(const T& v) const;
			// return the number of edges entering  v.
			// Precondition: v is a vertex in the graph. if not,
			// the function throws the graphError exception

		int outDegree(const T& v) const;
			// return the number of edges leaving  v.
			// Precondition: v is a vertex in the graph. if not,
			// the function throws the graphError exception

      set<T> getNeighbors(const T& v) const;
			// return a set containing the neighbors of v.
			// Precondition: v is a vertex in the graph. if not,
			// the function throws the graphError exception

		void insertEdge(const T& v1, const T& v2, int w);
			// add the edge (v1,v2) with specified weight to the graph.
			// Precondition: v1 and v2 are vertices in the graph. if not,
			// the function throws the graphError exception
			// Postcondition: The number of edges increases by 1

		void insertVertex(const T& v);
			// insert v into the graph.
			// Precondition: v is a vertex in the graph. if not,
			// the function throws the graphError exception.
			// Postcondition: the number of vertices increases by 1

		void eraseEdge(const T& v1, const T& v2);
			// erase edge (v1,v2) from the graph
			// Precondition: v1 and v2 are vertices in the graph. if not,
			// the function throws the graphError exception.
			// Postcondition: The number of edges decreases by 1

      void eraseVertex(const T& v);
			// erase v from the graph
			// Precondition: v is a vertex in the graph. if not,
			// the function throws the graphError exception.
			// Postconditions: The number of vertices decreases by 1,
			// and the operation removes all edges to or from v

		void clear();
			// remove all the vertices and edges from the graph

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
			// iterator functions returns corresponding map iterator

// "d_galgs.h" implements the graph algorithms using inline code.
// this is necessary for the Borland C++ 5.5 compiler
#include "d_galgs.h"

/*
		LISTING OF THE PROTOTYPES FOR THE GRAPH ALGORITHMS

		friend istream& operator>> (istream& istr, graph<T>& g);
			// input a graph

		friend ostream& operator<< (ostream& ostr, const graph<T>& g);
			// output a graph

		friend set<T> bfs(graph<T>& g, const T& sVertex);
			// perform the breadth-first traversal from sVertex and
			// return the set of visited vertices

		friend int shortestPath(graph<T>& g, const T& sVertex,
										const T& eVertex, list<T>& path);
			// use the breadth-first traversal algorithm to determine the
			// minimum number of edges in any path from sVertex to eVertex
			// or -1 if no path exists. if a path exists, the list path
			// is the sequence of vertices

		friend int minimumPath(graph<T>& g, const T& sVertex, const T& eVertex,
									  list<T>& path);
			// find the path with minimum total weight from sVertex to eVertex
			// and return the minimum weight

		friend int minSpanTree(graph<T>& g, graph<T>& MST);
			// find the minimum spanning tree for the strongly connected digraph g

		friend bool acyclic(graph<T>& g);
			// determine if the graph is acyclic

		friend void dfsVisit(graph<T>& g, const T& sVertex, list<T>& dfsList,
									bool checkForCycle);
			// depth-first visit assuming a WHITE starting vertex. dfsList
			// contains the visited vertices in reverse order of finishing time.
			// when checkForCycle is true, the function throws an exception if
			// it detects a cycle

		friend void dfs(graph<T>& g, list<T>& dfsList);
			// depth-first search. dfsList contains all the graph vertices in the
			// reverse order of their finishing times

		friend void topologicalSort(graph<T>& g, list<T>& tlist);
			// find a topological sort of an acyclic graph

		friend graph<T> transpose(graph<T>& g);
			// return the transpose of the graph

		friend void strongComponents(graph<T>& g, vector<set<T> >& component);
			// find the strong components of the graph
*/

	private:
      typedef map<T,int> vertexMap;

		vertexMap vtxMap;
			// store vertex in a map with its name as the key and the index
			// of the corresponding vertexInfo object in the vInfo
			// vector as the value

		vector<vertexInfo<T> > vInfo;
			// list of vertexInfo objects corresponding to the vertices

		int numVertices;
		int numEdges;
			// current size (vertices and edges) of the graph

		stack<int> availStack;
			// availability stack for storing unused indices in vInfo

		int getvInfoIndex(const T& v) const;
     		// uses vtxMap to obtain the index of v in vInfo
};

// uses vtxMap to obtain the index of v in vInfo
template <typename T>
int graph<T>::getvInfoIndex(const T& v) const
{
	// iter used in map lookup
	typename vertexMap::const_iterator iter;
	// index that is returned
	int pos;

	// find the map entry with key v
	iter = vtxMap.find(v);

	// make sure v is in the map
	if (iter == vtxMap.end())
		pos = -1;
	else
		// the index into vInfo is the value of the map entry
		pos = (*iter).second;

	return pos;
}

// constructor. initialize numVertices and numEdges to 0
template <typename T>
graph<T>::graph(): numVertices(0), numEdges(0)
{}

// copy constructor
template <typename T>
graph<T>::graph(const graph<T>& g)
{
	*this = g;	// copy g to current object
}

// overloaded assignment operator
template <typename T>
graph<T>& graph<T>::operator= (const graph<T>& rhs)
{
	typename vertexMap::iterator mi;

	// can't copy a graph to itself
	if (this == &rhs)
		return *this;

	// copy rhs to current object
	vtxMap = rhs.vtxMap;
	vInfo = rhs.vInfo;
	numVertices = rhs.numVertices;
	numEdges = rhs.numEdges;
	availStack = rhs.availStack;

	// update each vtxMapLoc value of objects in vInfo so it points
	// to a key-value pair in the copy of rhs.vtxMap and not rhs.vtxMap
	for (mi=vtxMap.begin();mi != vtxMap.end();mi++)
			vInfo[(*mi).second].vtxMapLoc = mi;

	return *this;
}

// ATTRIBUTE TESTING FUNCTIONS

template <typename T>
int graph<T>::numberOfVertices() const
{
	return numVertices;
}

template <typename T>
int graph<T>::numberOfEdges() const
{
	return numEdges;
}

template <typename T>
bool graph<T>::empty() const
{
	return numVertices == 0;
}

// ACCESS MEMBER FUNCTIONS

// return the weight of the edge (v1, v2). if the edge
// does not exist, return -1
template <typename T>
int graph<T>::getWeight(const T& v1, const T& v2) const
{
	// find the vInfo indices for the two vertices
	int pos1=getvInfoIndex(v1), pos2=getvInfoIndex(v2);

	// check for an error
	if (pos1 == -1 || pos2 == -1)
		// if v1 or v2 not in list of vertices, throw an exception
		throw graphError("graph getWeight(): vertex not in the graph");

	// construct an alias for the edge list in vInfo[pos1]
	const set<neighbor>& edgeSet = vInfo[pos1].edges;
	set<neighbor>::const_iterator setIter;

	// search for pos2 in the edge list and return its weight
	// if found; otherwise, return -1 to indicate that the
	// edge does not exist
	if ((setIter = edgeSet.find(neighbor(pos2))) != edgeSet.end())
		return (*setIter).weight;
	else
		return -1;
}

template <typename T>
void graph<T>::setWeight(const T& v1, const T& v2, int w)
{
	// find the vInfo indices for the two vertices
	int pos1=getvInfoIndex(v1), pos2=getvInfoIndex(v2);

	// check for an error
	if (pos1 == -1 || pos2 == -1)
		// if v1 or v2 not in list of vertices, throw an exception
		throw graphError("graph setWeight(): vertex not in the graph");

	// construct an alias for the edge list in vInfo[pos1]
	set<neighbor>& edgeSet = vInfo[pos1].edges;
	set<neighbor>::iterator setIter;

	// search for pos2 in the edge list and update its weight.
	// if the edge does not exist, throw an exception
	if ((setIter = edgeSet.find(neighbor(pos2))) != edgeSet.end())
		(*setIter).weight = w;
	else
		throw graphError("graph setWeight(): edge not in the graph");
}

// return the number of edges entering  v
template <typename T>
int graph<T>::inDegree(const T& v) const
{
	// find the vInfo index for v
	int pos=getvInfoIndex(v);

	if (pos != -1)
		// in-degree is stored in vInfo[pos]
		return vInfo[pos].inDegree;
	else
		// throw an exception
		throw graphError("graph inDegree(): vertex not in the graph");
}

// return the number of edges leaving  v
template <typename T>
int graph<T>::outDegree(const T& v) const
{
	// find the vInfo index for v
	int pos=getvInfoIndex(v);

	if (pos != -1)
		// out-degree is number of elements in the edge set
		return vInfo[pos].edges.size();
	else
		// throw an exception
		throw graphError("graph outDegree(): vertex not in the graph");
}

// return the list of all adjacent vertices
template <typename T>
set<T> graph<T>::getNeighbors(const T& v) const
{
	// set returned
	set<T> adjVertices;

	// obtain the position of v from the map
	int pos = getvInfoIndex(v);

	// if v not in list of vertices, throw an exception
	if (pos == -1)
		throw
			graphError("graph getNeighbors(): vertex not in the graph");

	// construct an alias for the set of edges in vertex pos
	const set<neighbor>& edgeSet = vInfo[pos].edges;
	// use setIter to traverse the edge set
	set<neighbor>::const_iterator setIter;

	// index of vertexInfo object corresponding to an adjacent vertex
	int aPos;

	for (setIter=edgeSet.begin(); setIter != edgeSet.end(); setIter++)
	{	// "(*setIter).dest" is index into vInfo
		aPos = (*setIter).dest;
		// insert vertex data into a set. vInfo[aPos].vtxMapLoc"
		// is a map iterator. dereference it to access the vertex
		adjVertices.insert ((*vInfo[aPos].vtxMapLoc).first);
	}

   return adjVertices;
}

// GRAPH MODIFICATION MEMBER FUNCTIONS

// add the edge (v1,v2) with specified weight to the graph
template <typename T>
void graph<T>::insertEdge(const T& v1,
								  const T& v2, int w)
{
	// obtain the vInfo indices
	int pos1=getvInfoIndex(v1), pos2=getvInfoIndex(v2);

	// check for an error
	if (pos1 == -1 || pos2 == -1)
		// if v1 or v2 not in list of vertices, throw an exception
		throw graphError("graph insertEdge(): vertex not in the graph");
	else if (pos1 == pos2)
		// we do not allow self-loops
		throw graphError("graph insertEdge(): self-loops are not allowed");


	// attempt to insert edge (pos2,w) into the edge set of vertex pos1
	pair<set<neighbor>::iterator, bool> result =
			vInfo[pos1].edges.insert(neighbor(pos2,w));

	// make sure edge was not already in the set
	if (result.second)
	{
		// increment the number of edges
		numEdges++;
		// the in-degree of v2 is one more
		vInfo[pos2].inDegree++;
	}
}

// insert v into the graph
template <typename T>
void graph<T>::insertVertex(const T& v)
{
	int index;

	// attempt to insert v into the map with index 0.
	// if successful, insert an iterator pointing at it
	// into the vertex list at location index. index is obtained
	// from the availability stack or by creating a new entry
	// at the back of the vector. fix the map entry to refer
	// to index and increment numVertices. if the insertion did
	// not take place, the vertex already exists. generate an
	// exception
	pair<typename vertexMap::iterator, bool> result =
		vtxMap.insert(typename vertexMap::value_type(v,0));
	if (result.second)
	{
		// see if there is an entry in vInfo freed by an earlier
		// call to eraseVertex()
		if (!availStack.empty())
		{
			// yes. get its index
			index = availStack.top();
			availStack.pop();
			// call to constructor builds a empty edge set
			vInfo[index] = vertexInfo<T>(result.first);
		}
		else
		{
			// no. we'll have to increase the size of vInfo
			vInfo.push_back(vertexInfo<T>(result.first));
			index = numVertices;
		}

		(*result.first).second = index;
		numVertices++;
	}
	else
		throw graphError("graph insertVertex(): vertex already in the graph");
}

// erase edge (v1,v2) from the graph
template <typename T>
void graph<T>::eraseEdge(const T& v1, const T& v2)
{
	// obtain the indices of v1 and v2 in vInfo
	int pos1=getvInfoIndex(v1), pos2=getvInfoIndex(v2);

	// check for an error
	if (pos1 == -1 || pos2 == -1)
		// if v1 or v2 not in list of vertices, throw an exception
		throw graphError("graph eraseEdge(): vertex not in the graph");

	// construct an alias to the edge set of vInfo[pos1]
	set<neighbor>& edgeSet = vInfo[pos1].edges;
	set<neighbor>::iterator setIter;

	// search for pos2 in the edge set
	setIter = edgeSet.find(neighbor(pos2));
	// if pos2 is in the set, erase it; otherwise, output an
	// error message
	if (setIter != edgeSet.end())
	{
		edgeSet.erase(setIter);
		// the in-degree of v2 is one less
		vInfo[pos2].inDegree--;
		numEdges--;
	}
	else
		throw graphError("graph eraseEdge(): edge not in the graph");
}

template <typename T>
void graph<T>::eraseVertex(const T& v)
{
	// use to search for and remove v from the map
	typename vertexMap::iterator mIter;
	// pos is index of v in the vertex list
	int pos, j;
	// used in removal of edges to v
	set<neighbor>::iterator sIter;

	// search the map for the key v
	mIter = vtxMap.find(v);
	// if vertex is not present, terminate the erase
	if (mIter == vtxMap.end())
		// if v not in list of vertices, throw an exception
		throw graphError("graph eraseVertex(): vertex not in the graph");

	// obtain the index of v in vInfo
	pos = (*mIter).second;

	// erase vertex from the map and decrement number of vertices
	vtxMap.erase(mIter);
	numVertices--;

	// mark the vertex entry in vInfo as not occupied. the index pos is now
	// available. push it on the availability stack for use later if we
	// insert a vertex
	vInfo[pos].occupied = false;
	availStack.push(pos);

	// cycle through vInfo and remove all edges going to v
	for (j=0; j < vInfo.size(); j++)
		// skip all unoccupied entries, including pos
		if (vInfo[j].occupied)
		{
			// construct an alias for the set vInfo[j].edges
			set<neighbor>& edgeSet = vInfo[j].edges;

			sIter = edgeSet.begin();
			// cycle through the edge set
			while (sIter != edgeSet.end())
				if ((*sIter).dest == pos)
				{
					// found pos. remove it from the set and
					// decrement the edge count
					edgeSet.erase(sIter);
					numEdges--;
					break;
				}
				else
					// took no action. just move forward
					sIter++;
		}

	// decrement numEdges by the number of edges for vertex v
	numEdges -= vInfo[pos].edges.size();

	// the in-degree for all of v's neighbors must be decreased by 1
	set<neighbor>& edgesFromv = vInfo[pos].edges;
	for (sIter=edgesFromv.begin(); sIter != edgesFromv.end(); sIter++)
	{
		j = (*sIter).dest;
		vInfo[j].inDegree--;
	}

	// clear the edge set. construct an alias for vInfo[pos].edges
	// and use erase to clear the set
	set<neighbor>& edgeSet = vInfo[pos].edges;
	edgeSet.erase(edgeSet.begin(), edgeSet.end());
}

// erase the graph
template <typename T>
void graph<T>::clear()
{
	// clear the vertex list, vertex map and the
	// availability stack
	vInfo.erase(vInfo.begin(), vInfo.end());
	vtxMap.erase(vtxMap.begin(), vtxMap.end());
	while(!availStack.empty())
		availStack.pop();

	// set the number of vertices and edges to 0
	numVertices = 0;
	numEdges = 0;
}

// ITERATOR FUNCTIONS

// each graph iterator function returns
// the corresponding map iterator
template <typename T>
typename graph<T>::iterator graph<T>::begin()
{
  return graph<T>::iterator(vtxMap.begin());
}

template <typename T>
typename graph<T>::iterator graph<T>::end()
{
	return graph<T>::iterator(vtxMap.end());
}

template <typename T>
typename graph<T>::const_iterator graph<T>::begin() const
{
	return graph<T>::iterator(vtxMap.begin());
}

template <typename T>
typename graph<T>::const_iterator graph<T>::end() const
{
  return graph<T>::iterator(vtxMap.end());
}

#endif	// GRAPH_CLASS
