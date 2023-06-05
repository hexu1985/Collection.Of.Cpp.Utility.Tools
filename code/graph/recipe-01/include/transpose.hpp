#pragma once

#include <set>

// return the transpose of the graph
template <typename T>
Graph<T> transpose(Graph<T>& g)
{
    int i, n = g.vInfo.size();
    std::set<Neighbor>::iterator setIter;
    // initialize gt as a copy of g
    Graph<T> gt = g;

    // clear the adjacency set of each vertex in gt and set the in-degree
    // of each vertex to 0
    for (i=0;i < n;i++)
    {
        gt.vInfo[i].edges.erase(gt.vInfo[i].edges.begin(),
                                        gt.vInfo[i].edges.end());
        gt.vInfo[i].inDegree = 0;
    }

    // assign the edges of gt as the reverse of those in g
    for (i=0; i < n; i++)
        if (g.vInfo[i].occupied)
        {
            std::set<Neighbor>& s = g.vInfo[i].edges;

            // edge from index i to index dest in vInfo for g must be an edge
            // from dest to i with the same weight in gt. increment the in-degree
            // at index i
            for (setIter = s.begin(); setIter != s.end(); setIter++)
            {
                gt.vInfo[(*setIter).dest].edges.insert(Neighbor(i,(*setIter).weight));
                gt.vInfo[i].inDegree++;
            }
        }

    return gt;
}
