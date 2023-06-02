#pragma once

#include "min_info.hpp"
#include <list>
#include <vector>
#include <queue>

// find the path with minimum total weight from sVertex to eVertex
// and return the minimum weight
template <typename T>
int minimum_path(Graph<T>& g, const T& sVertex, const T& eVertex,
        std::list<T>& path)
{
    // heap (priority queue) that stores MinInfo objects
    std::priority_queue<MinInfo,std::vector<MinInfo>,std::greater<MinInfo>>  minPathPQ;

    // used when inserting MinInfo entries
    // into the priority queue or erasing entries
    MinInfo vertexData;
    // adj is edge set of vertex we are visiting. adjiter is used
    // to traverse adj
    std::set<Neighbor> adj;
    std::set<Neighbor>::iterator adjIter;

    bool foundMinPath = false;

    // index in vInfo for the starting and ending vertices
    // position in vInfo of vertex on a path from sVertex
    int pos_sVertex, pos_eVertex, currPos, destPos;

    // computed minimum weight
    int newMinWeight;

    // return value
    int returnValue;

    typename std::list<T>::iterator pathIter;

    // initialize all vertices to unmarked (WHITE) and dataValue
    // variables to INFINITY
    for (size_t i = 0;i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
        {
            g.vInfo[i].color = VertexInfo<T>::WHITE;
            g.vInfo[i].dataValue = INFINITY;
        }

    // obtain the starting and ending indices
    pos_sVertex = g.GetVertexInfoIndex(sVertex);
    pos_eVertex = g.GetVertexInfoIndex(eVertex);

    if (pos_sVertex == -1 || pos_eVertex == -1)
        throw GraphError("graph minimum_path(): vertex not in the graph");


    // formulate initial priority queue entry
    vertexData.endV = pos_sVertex;

    // path weight from sVertex to sVertex is 0
    vertexData.pathWeight = 0;

    // update dataValue in vInfo and set sVertex as parent
    g.vInfo[pos_sVertex].dataValue = 0;
    g.vInfo[pos_sVertex].parent = pos_sVertex;

    // insert starting vertex into the priority queue
    minPathPQ.push(vertexData);

    // process vertices until we find a minimum path to
    // eVertex or the priority queue is empty
    while (!minPathPQ.empty())
    {
        // delete a priority queue entry and record its
        // vertex and path weight from sVertex.
        vertexData = minPathPQ.top();
        minPathPQ.pop();
        currPos = vertexData.endV;

        // if we are at eVertex, we have found the minimum
        // path from sVertex to eVertex
        if (currPos == pos_eVertex)
        {
            foundMinPath = true;
            break;
        }

        if (g.vInfo[currPos].color != VertexInfo<T>::BLACK)
        {
            // mark the vertex so we don't look at it again
            g.vInfo[currPos].color = VertexInfo<T>::BLACK;

            // find all neighbors of the current vertex pos. for each
            // neighbor that has not been visited, generate a MinInfo
            // object and insert it into the priority queue provided the
            // total weight to get to the neighbor is better than the
            // current dataValue in vInfo
            adj = g.vInfo[currPos].edges;
            for(adjIter = adj.begin();adjIter != adj.end();
                    adjIter++)
            {
                destPos = (*adjIter).dest;

                if (g.vInfo[destPos].color == VertexInfo<T>::WHITE)
                {
                    // compare total weight of adding edge to dataValue
                    if ((newMinWeight = (g.vInfo[currPos].dataValue +
                                    (*adjIter).weight)) < g.vInfo[destPos].dataValue)
                    {
                        // add minVertexInfo object for new vertex and update
                        // dataValue in vInfo
                        vertexData.endV = destPos;
                        vertexData.pathWeight = newMinWeight;
                        g.vInfo[destPos].dataValue = newMinWeight;
                        g.vInfo[destPos].parent = currPos;
                        minPathPQ.push(vertexData);
                    }	// end "if" that checks weights
                }	// end "if" that checks if neighbor is not marked
            }	// end "for"
        }	// end "if" vertex not already marked
    }	// end "while"

    // clear path and setup return
    path.erase(path.begin(), path.end());
    if (foundMinPath)
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

