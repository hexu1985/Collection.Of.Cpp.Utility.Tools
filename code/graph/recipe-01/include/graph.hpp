#pragma once

#include <set>				// set class
#include <map>				// ist classmap class
#include <vector>			// vector class
#include <list>			// list class
#include <stack>			// stack class
#include <queue>			// queue class
#include <functional>	// less<T>

#include "graph_error.hpp"

// largest positive integer on the machine
const int INFINITY = (int)((unsigned int)~0 >> 1);

class Neighbor
{
public:
    // index of the destination vertex in the vector vInfo of vertex
    // properties
    int dest;
    // weight of this edge
    int weight;

    // constructor
    Neighbor(int d=0, int c=0): dest(d), weight(c)
    {}

    // operators for the neighbor class that compare the
    // destination vertices
    friend bool operator< (const Neighbor& lhs, const Neighbor& rhs)
    {
        return lhs.dest < rhs.dest;
    }

    friend bool operator== (const Neighbor& lhs, const Neighbor& rhs)
    {
        return lhs.dest == rhs.dest;
    }
};

// maintains vertex properties, including its set of
// neighbors
template <typename T>
class VertexInfo
{
public:
    // used by graph algorithms
    enum VertexColor { WHITE, GRAY, BLACK };

    // iterator pointing at a pair<T,int> object in the vertex map
    typename std::map<T,int>::iterator vtxMapLoc;

    // set of adjacent (neighbor) objects for the current vertex
    std::set<Neighbor> edges;

    /// maintains the in-degree of the vertex
    int inDegree;

    // indicates whether the object currently represents a vertex
    bool occupied;

    // indicate if a vertex is marked in an algorithm that traverses
    // the vertices of a graph
    VertexColor color;

    // available to algorithms for storing relevant data values
    int dataValue;

    // available to graph algorithms; holds parent which is
    // a vertex that has an edge terminating in the current vertex
    int parent;

    // default constructor
    VertexInfo(): inDegree(0), occupied(true)
    {}

    // constructor with iterator pointing to the vertex in the map
    VertexInfo(typename std::map<T,int>::iterator iter):
        vtxMapLoc(iter), inDegree(0), occupied(true)
    {}
};

template <typename T>
class Graph
{
public:
    class const_iterator: public std::map< T, int >::const_iterator
    {
    public:
        const_iterator()
        {}

        // converts a map iterator to a graph iterator
        const_iterator(typename std::map<T,int>::const_iterator i)
        {
            *((typename std::map< T, int >::const_iterator *)this) = i;
        }

        // return the vertex pointed to by the iterator
        const T& operator* () const
        {
            typename std::map<T,int>::const_iterator p = *this;

            return (*p).first;
        }
    };

    typedef const_iterator iterator;

    Graph();
    // constructor. initialize numVertices and numEdges to 0

    Graph(const Graph<T>& g);
    // copy constructor

    Graph<T>& operator= (const Graph<T>& rhs);
    // overloaded assignment operator

    int NumberOfVertices() const;
    // return the number of vertices in the graph

    int NumberOfEdges() const;
    // return the number of edges in the graph

    bool empty() const;
    // is the graph empty?

    int GetWeight(const T& v1, const T& v2) const;
    // return the weight of the edge (v1, v2). if the edge.
    // does not exist, return -1
    // Precondition: v1 and v2 are vertices in the graph. if not
    // the function throws the graphError exception

    void SetWeight(const T& v1, const T& v2, int w);
    // update the weight of edge (v1, v2).
    // Precondition: v1 and v2 are vertices in the graph. if not,
    // the function throws the graphError exception
    // Postcondition: the weight of vertex (v1,v2) is w

    int InDegree(const T& v) const;
    // return the number of edges entering  v.
    // Precondition: v is a vertex in the graph. if not,
    // the function throws the graphError exception

    int OutDegree(const T& v) const;
    // return the number of edges leaving  v.
    // Precondition: v is a vertex in the graph. if not,
    // the function throws the graphError exception

    std::set<T> GetNeighbors(const T& v) const;
    // return a set containing the neighbors of v.
    // Precondition: v is a vertex in the graph. if not,
    // the function throws the graphError exception

    void InsertEdge(const T& v1, const T& v2, int w);
    // add the edge (v1,v2) with specified weight to the graph.
    // Precondition: v1 and v2 are vertices in the graph. if not,
    // the function throws the graphError exception
    // Postcondition: The number of edges increases by 1

    void InsertVertex(const T& v);
    // insert v into the graph.
    // Precondition: v is a vertex in the graph. if not,
    // the function throws the graphError exception.
    // Postcondition: the number of vertices increases by 1

    void EraseEdge(const T& v1, const T& v2);
    // erase edge (v1,v2) from the graph
    // Precondition: v1 and v2 are vertices in the graph. if not,
    // the function throws the graphError exception.
    // Postcondition: The number of edges decreases by 1

    void EraseVertex(const T& v);
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

    const std::vector<VertexInfo<T>>& InfoOfVertices() const;

private:
    typedef std::map<T,int> vertexMap;

    vertexMap vtxMap;
    // store vertex in a map with its name as the key and the index
    // of the corresponding VertexInfo object in the vInfo
    // vector as the value

    std::vector<VertexInfo<T>> vInfo;
    // list of VertexInfo objects corresponding to the vertices

    int numVertices;
    int numEdges;
    // current size (vertices and edges) of the graph

    std::stack<int> availStack;
    // availability stack for storing unused indices in vInfo

    int GetVertexInfoIndex(const T& v) const;
    // uses vtxMap to obtain the index of v in vInfo
};

// uses vtxMap to obtain the index of v in vInfo
template <typename T>
int Graph<T>::GetVertexInfoIndex(const T& v) const
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
Graph<T>::Graph(): numVertices(0), numEdges(0)
{}

// copy constructor
template <typename T>
Graph<T>::Graph(const Graph<T>& g)
{
    *this = g;	// copy g to current object
}

// overloaded assignment operator
template <typename T>
Graph<T>& Graph<T>::operator= (const Graph<T>& rhs)
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
int Graph<T>::NumberOfVertices() const
{
    return numVertices;
}

template <typename T>
int Graph<T>::NumberOfEdges() const
{
    return numEdges;
}

template <typename T>
bool Graph<T>::empty() const
{
    return numVertices == 0;
}

// ACCESS MEMBER FUNCTIONS

// return the weight of the edge (v1, v2). if the edge
// does not exist, return -1
template <typename T>
int Graph<T>::GetWeight(const T& v1, const T& v2) const
{
    // find the vInfo indices for the two vertices
    int pos1=GetVertexInfoIndex(v1), pos2=GetVertexInfoIndex(v2);

    // check for an error
    if (pos1 == -1 || pos2 == -1)
        // if v1 or v2 not in list of vertices, throw an exception
        throw graphError("graph getWeight(): vertex not in the graph");

    // construct an alias for the edge list in vInfo[pos1]
    const std::set<Neighbor>& edgeSet = vInfo[pos1].edges;
    std::set<Neighbor>::const_iterator setIter;

    // search for pos2 in the edge list and return its weight
    // if found; otherwise, return -1 to indicate that the
    // edge does not exist
    if ((setIter = edgeSet.find(Neighbor(pos2))) != edgeSet.end())
        return (*setIter).weight;
    else
        return -1;
}

template <typename T>
void Graph<T>::SetWeight(const T& v1, const T& v2, int w)
{
    // find the vInfo indices for the two vertices
    int pos1=GetVertexInfoIndex(v1), pos2=GetVertexInfoIndex(v2);

    // check for an error
    if (pos1 == -1 || pos2 == -1)
        // if v1 or v2 not in list of vertices, throw an exception
        throw graphError("graph setWeight(): vertex not in the graph");

    // construct an alias for the edge list in vInfo[pos1]
    std::set<Neighbor>& edgeSet = vInfo[pos1].edges;
    std::set<Neighbor>::iterator setIter;

    // search for pos2 in the edge list and update its weight.
    // if the edge does not exist, throw an exception
    if ((setIter = edgeSet.find(Neighbor(pos2))) != edgeSet.end())
        (*setIter).weight = w;
    else
        throw graphError("graph setWeight(): edge not in the graph");
}

// return the number of edges entering  v
template <typename T>
int Graph<T>::InDegree(const T& v) const
{
    // find the vInfo index for v
    int pos=GetVertexInfoIndex(v);

    if (pos != -1)
        // in-degree is stored in vInfo[pos]
        return vInfo[pos].inDegree;
    else
        // throw an exception
        throw graphError("graph inDegree(): vertex not in the graph");
}

// return the number of edges leaving  v
template <typename T>
int Graph<T>::OutDegree(const T& v) const
{
    // find the vInfo index for v
    int pos=GetVertexInfoIndex(v);

    if (pos != -1)
        // out-degree is number of elements in the edge set
        return vInfo[pos].edges.size();
    else
        // throw an exception
        throw graphError("graph outDegree(): vertex not in the graph");
}

// return the list of all adjacent vertices
template <typename T>
std::set<T> Graph<T>::GetNeighbors(const T& v) const
{
    // set returned
    std::set<T> adjVertices;

    // obtain the position of v from the map
    int pos = GetVertexInfoIndex(v);

    // if v not in list of vertices, throw an exception
    if (pos == -1)
        throw
            graphError("graph getNeighbors(): vertex not in the graph");

    // construct an alias for the set of edges in vertex pos
    const std::set<Neighbor>& edgeSet = vInfo[pos].edges;
    // use setIter to traverse the edge set
    std::set<Neighbor>::const_iterator setIter;

    // index of VertexInfo object corresponding to an adjacent vertex
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
void Graph<T>::InsertEdge(const T& v1,
        const T& v2, int w)
{
    // obtain the vInfo indices
    int pos1=GetVertexInfoIndex(v1), pos2=GetVertexInfoIndex(v2);

    // check for an error
    if (pos1 == -1 || pos2 == -1)
        // if v1 or v2 not in list of vertices, throw an exception
        throw graphError("graph insertEdge(): vertex not in the graph");
    else if (pos1 == pos2)
        // we do not allow self-loops
        throw graphError("graph insertEdge(): self-loops are not allowed");


    // attempt to insert edge (pos2,w) into the edge set of vertex pos1
    std::pair<std::set<Neighbor>::iterator, bool> result =
        vInfo[pos1].edges.insert(Neighbor(pos2,w));

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
void Graph<T>::InsertVertex(const T& v)
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
    std::pair<typename vertexMap::iterator, bool> result =
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
            vInfo[index] = VertexInfo<T>(result.first);
        }
        else
        {
            // no. we'll have to increase the size of vInfo
            vInfo.push_back(VertexInfo<T>(result.first));
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
void Graph<T>::EraseEdge(const T& v1, const T& v2)
{
    // obtain the indices of v1 and v2 in vInfo
    int pos1=GetVertexInfoIndex(v1), pos2=GetVertexInfoIndex(v2);

    // check for an error
    if (pos1 == -1 || pos2 == -1)
        // if v1 or v2 not in list of vertices, throw an exception
        throw graphError("graph eraseEdge(): vertex not in the graph");

    // construct an alias to the edge set of vInfo[pos1]
    std::set<Neighbor>& edgeSet = vInfo[pos1].edges;
    std::set<Neighbor>::iterator setIter;

    // search for pos2 in the edge set
    setIter = edgeSet.find(Neighbor(pos2));
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
void Graph<T>::EraseVertex(const T& v)
{
    // use to search for and remove v from the map
    typename vertexMap::iterator mIter;
    // pos is index of v in the vertex list
    int pos, j;
    // used in removal of edges to v
    std::set<Neighbor>::iterator sIter;

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
            std::set<Neighbor>& edgeSet = vInfo[j].edges;

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
    std::set<Neighbor>& edgesFromv = vInfo[pos].edges;
    for (sIter=edgesFromv.begin(); sIter != edgesFromv.end(); sIter++)
    {
        j = (*sIter).dest;
        vInfo[j].inDegree--;
    }

    // clear the edge set. construct an alias for vInfo[pos].edges
    // and use erase to clear the set
    std::set<Neighbor>& edgeSet = vInfo[pos].edges;
    edgeSet.erase(edgeSet.begin(), edgeSet.end());
}

// erase the graph
template <typename T>
void Graph<T>::clear()
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
typename Graph<T>::iterator Graph<T>::begin()
{
    return Graph<T>::iterator(vtxMap.begin());
}

template <typename T>
typename Graph<T>::iterator Graph<T>::end()
{
    return Graph<T>::iterator(vtxMap.end());
}

template <typename T>
typename Graph<T>::const_iterator Graph<T>::begin() const
{
    return Graph<T>::iterator(vtxMap.begin());
}

template <typename T>
typename Graph<T>::const_iterator Graph<T>::end() const
{
    return Graph<T>::iterator(vtxMap.end());
}

template <typename T>
const std::vector<VertexInfo<T>>& Graph<T>::InfoOfVertices() const
{
    return vInfo;
}
