#pragma once

#include <vector>
#include <list>
#include <set>

#include "dfs.hpp"
#include "transpose.hpp"

// find the strong components of the graph
template <typename T>
void strong_components(Graph<T>& g, std::vector<std::set<T>>& component)
{
    // list of vertices visited by dfs() for graph g
    std::list<T> dfsGList;

    // list of vertices visited by dfs_visit() for g transpose
    std::list<T> dfsGTList;

    // used to scan dfsGList and dfsGTList objects
    typename std::list<T>::iterator gIter, gtIter;

    // transpose of the graph
    Graph<T> gt;

    // set for an individual strong component
    std::set<T> scSet;

    // clear the return vector
    component.resize(0);

    // execute depth-first tranversal of g
    dfs(g, dfsGList);

    // compute gt
    gt = transpose(g);

    // initialize all vertices in gt to WHITE (unvisited)
    for (size_t i=0;i < gt.vInfo.size(); i++)
        if (gt.vInfo[i].occupied)
            gt.vInfo[i].color = VertexInfo<T>::WHITE;

    // call dfs_visit() for gt from vertices in dfsGList
    gIter = dfsGList.begin();
    while(gIter != dfsGList.end())
    {
        // call dfs_visit() only if vertex has not been visited
        if (gt.vInfo[gt.GetVertexInfoIndex(*gIter)].color ==
                         VertexInfo<T>::WHITE)
        {
            // clear dfsGTList and scSet
            dfsGTList.erase(dfsGTList.begin(), dfsGTList.end());
            scSet.erase(scSet.begin(), scSet.end());

            // do dfs_visit() in gt for starting vertex *gIter
            dfs_visit(gt, *gIter, dfsGTList, false);

            // copy vertices from the list to set scSet
            for (gtIter = dfsGTList.begin(); gtIter != dfsGTList.end();
                                gtIter++)
                scSet.insert(*gtIter);

            // add strong component set to the vector
            component.push_back(scSet);
        }
        gIter++;
    }
}
