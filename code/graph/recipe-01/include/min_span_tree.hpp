#pragma once

#include "min_info.hpp"
#include <vector>
#include <queue>

// find the minimum spanning tree for the strongly connected digraph g
template <typename T>
int min_span_tree(Graph<T>& g, Graph<T>& MST)
{
    // priority queue that stores MinInfo objects
    std::priority_queue<MinInfo, std::vector<MinInfo>, std::greater<MinInfo>>  minTreePQ;

    // used when inserting MinInfo entries
    // into the priority queue or erasing entries
    MinInfo vertexData;

    // adj is adjacency set of vertex we are visiting. adjIter
    // is an iterator that scans the list
    std::set<Neighbor> adj;
    std::set<Neighbor>::iterator adjIter;

    // size of the minimum spanning tree
    int minSpanTreeSize = 0;

    // index of a starting vertex.
    int pos_sVertex = -1, currPos, destPos, parentPos;

    // current minimum total weight for spanning tree
    int minSpanTreeWeight = 0;

    // initialize all vertices unmarked and their dataValue fields
    // to infinity;  select the first vertex as the	starting vertex;
    for (size_t i = 0;i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
        {
            if (pos_sVertex == -1)
                pos_sVertex = i;
            g.vInfo[i].color = VertexInfo<T>::WHITE;
            g.vInfo[i].dataValue = INFINITY;
        }

    // create MinInfo object for starting vertex
    vertexData.endV = pos_sVertex;

    // total weight of spanning tree with only the
    // starting vertex is 0
    vertexData.pathWeight = 0;

    // update dataValue in vInfo and set rVertex as parent
    g.vInfo[pos_sVertex].dataValue = 0;
    g.vInfo[pos_sVertex].parent = pos_sVertex;

    // insert starting vertex into the priority queue
    minTreePQ.push(vertexData);

    // add vertices until we span the entire graph
    for (;;)
    {
        // delete a priority queue entry
        vertexData = minTreePQ.top();
        minTreePQ.pop();
        currPos = vertexData.endV;

        // if vertex is not part of the new graph (unvisited)
        // add the weight of the edge to the total tree we3ight
        // and increment the number of vertices in the tree
        if (g.vInfo[currPos].color == VertexInfo<T>::WHITE)
        {
            minSpanTreeWeight += vertexData.pathWeight;
            minSpanTreeSize++;

            // if we spanned all vertices, break
            if (minSpanTreeSize == g.NumberOfVertices())
                break;

            // mark the vertex BLACK so we don't look at it again.
            // set dataValue in vInfo vector to current
            // minimum tree weight
            g.vInfo[currPos].color = VertexInfo<T>::BLACK;

            // find all unmarked neighbors of the vertex.
            // adjIter is a set iterator pointing at the edge corresponding to
            // vertices with index currPos and destPos
            adj = g.vInfo[currPos].edges;
            for(adjIter = adj.begin();adjIter != adj.end();	adjIter++)
            {
                destPos = (*adjIter).dest;

                // if neighbor is unmarked, check whether adding the new
                // edge to the tree is better than using the current edge
                if (g.vInfo[destPos].color == VertexInfo<T>::WHITE)
                {
                    if ((*adjIter).weight < g.vInfo[destPos].dataValue)
                    {
                        // if new edge is a better connection, create MinInfo object
                        // for new vertex. update dataValue and parent variables
                        // in vertexInfo
                        vertexData.endV = destPos;
                        vertexData.pathWeight = (*adjIter).weight;
                        g.vInfo[destPos].dataValue = (*adjIter).weight;
                        g.vInfo[destPos].parent = currPos;
                        minTreePQ.push(vertexData);
                    }
                }
            }
        }
    }

    // vertices for edges in MST
    T vA, vB;

    // clear tree and then add all of the vertices
    MST.clear();

    for(size_t i = 0; i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
            MST.InsertVertex((*(g.vInfo[i]).vtxMapLoc).first);

    // add the edges to the minimum spanning tree
    for (size_t i = pos_sVertex+1; i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
        {
            parentPos = g.vInfo[i].parent;
            vA = (*(g.vInfo[parentPos]).vtxMapLoc).first;
            vB = (*(g.vInfo[i]).vtxMapLoc).first;
            MST.InsertEdge(vA,vB, g.GetWeight(vA,vB));
        }

    return minSpanTreeWeight;
}
