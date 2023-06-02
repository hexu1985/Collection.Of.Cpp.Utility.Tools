#pragma once

#include <set>
#include <queue>

// perform the breadth-first traversal from sVertex and
// return the set of visited vertices
template <typename T>
std::set<T> bfs(Graph<T>& g, const T& sVertex)
{
    // BFS uses a queue to store indices of adjacent vertices from vInfo
    std::queue<int> visitQueue;

    // set of vertices in BFS
    std::set<T> visitSet;

    // use to store indices in vInfo
    int currVertex, neighborVertex;

    // use to search edge sets for unvisited vertices
    std::set<Neighbor>::iterator adj;

    // find the index of the starting vertex
    currVertex = g.GetVertexInfoIndex(sVertex);

    // check for a nonexistent starting vertex
    if (currVertex == -1)
        throw GraphError("graph bfs(): vertex not in the graph");

    // initialize all vertices in the graph to unvisited (WHITE)
    for (size_t i=0;i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
            g.vInfo[i].color = VertexInfo<T>::WHITE;

    visitQueue.push(currVertex);   // initialize the queue

    while (!visitQueue.empty())
    {
        // remove a vertex from the queue
        currVertex = visitQueue.front();
        visitQueue.pop();
        // indicate that the vertex has been visited
        g.vInfo[currVertex].color = VertexInfo<T>::BLACK;

        // put the vertex in visitSet
        visitSet.insert((*(g.vInfo[currVertex].vtxMapLoc)).first);

        // create an alias for the edge set of currVertex
        std::set<Neighbor>& edgeSet = g.vInfo[currVertex].edges;
        // sequence through the edge set and look for vertices
        // that have not been visited
        for (adj = edgeSet.begin(); adj != edgeSet.end(); adj++)
        {
            neighborVertex = (*adj).dest;

            if (g.vInfo[neighborVertex].color == VertexInfo<T>::WHITE)
            {
                g.vInfo[neighborVertex].color = VertexInfo<T>::GRAY;
                visitQueue.push(neighborVertex);
            }
        }
    }

    return visitSet;
}
