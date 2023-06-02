#pragma once

#include <list>
#include <queue>
#include <set>

// use the breadth-first traversal algorithm to determine the
// minimum number of edges in any path from sVertex to eVertex
// or -1 if no path exists. if a path exists, the list path
// is the sequence of vertices
template <typename T>
int shortest_path(Graph<T>& g, const T& sVertex,
        const T& eVertex, std::list<T>& path)
{
    // queue stores vertices as vInfo indices
    std::queue<int>  visitQueue;

    // eIter scans the vertices in an adjacency set
    std::set<Neighbor>::iterator eIter;

    // flag set true when scan identifies eVertex as a neighbor
    bool foundShortestPath = false;

    // index in vInfo for the source and destination vertices
    // and the index for current vertex and a neighbor
    int pos_sVertex, pos_eVertex, currPos, neighborPos;

    int returnValue;

    // initialize all vertices to undiscoved (WHITE)
    for (size_t i = 0;i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
            g.vInfo[i].color = VertexInfo<T>::WHITE;

    // obtain the starting and ending indices
    pos_sVertex = g.GetVertexInfoIndex(sVertex);
    pos_eVertex = g.GetVertexInfoIndex(eVertex);

    if (pos_sVertex == -1 || pos_eVertex == -1)
        throw GraphError("graph shortest_path(): vertex not in "
                "the graph");

    g.vInfo[pos_sVertex].parent = pos_sVertex;
    g.vInfo[pos_sVertex].dataValue = 0;

    // insert starting vertex into the queue
    visitQueue.push(pos_sVertex);

    while (!visitQueue.empty() && !foundShortestPath)
    {
        // delete a queue entry, and color it BLACK
        currPos = visitQueue.front();
        visitQueue.pop();
        g.vInfo[currPos].color = VertexInfo<T>::BLACK;

        // if we are at eVertex, we have found the shortest
        // path from sVertex to eVertex
        if (currPos == pos_eVertex)
            foundShortestPath = true;
        else
        {
            // create an alias for the adjacency set of currPos
            std::set<Neighbor>& edgeSet = g.vInfo[currPos].edges;

            // for all undiscovered neighbors, update the dataValue,
            // color, and parent fields in the vertexInfo object.
            for (eIter = edgeSet.begin(); eIter != edgeSet.end(); eIter++)
            {
                neighborPos = (*eIter).dest;

                if (g.vInfo[neighborPos].color == VertexInfo<T>::WHITE)
                {
                    g.vInfo[neighborPos].dataValue =
                        g.vInfo[currPos].dataValue + 1;
                    g.vInfo[neighborPos].parent = currPos;
                    g.vInfo[neighborPos].color = VertexInfo<T>::GRAY;
                    // add neighbor vertex to the queue
                    visitQueue.push(neighborPos);
                }
            }
        }
    }

    // clear path and find the sequence of vertices
    // from sVertex to eVertex
    path.erase(path.begin(), path.end());
    if (foundShortestPath)
    {
        currPos = pos_eVertex;
        while (currPos != pos_sVertex)
        {
            path.push_front((*(g.vInfo[currPos].vtxMapLoc)).first);
            currPos = g.vInfo[currPos].parent;
        }
        path.push_front(sVertex);
        returnValue = g.vInfo[pos_eVertex].dataValue;
    }
    else
        returnValue = -1;

    return returnValue;
}
