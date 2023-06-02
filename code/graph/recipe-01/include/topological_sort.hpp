#pragma once

#include "dfs_visit.hpp"
#include <list>

// find a topological sort of an acyclic graph
template <typename T>
void topological_sort(Graph<T>& g, std::list<T>& tlist)
{
    // clear the list that will contain the sort
    tlist.erase(tlist.begin(), tlist.end());

    for (size_t i=0;i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
            g.vInfo[i].color = VertexInfo<T>::WHITE;

    // cycle through the vertices, calling dfsVisit() for each
    // WHITE vertex. check for a cycle
    try
    {
        for (size_t i=0;i < g.vInfo.size(); i++)
            if (g.vInfo[i].occupied && g.vInfo[i].color == VertexInfo<T>::WHITE)
                dfs_visit(g, (*(g.vInfo[i].vtxMapLoc)).first, tlist, true);
    }
    catch(GraphError&)
    {
        throw GraphError("graph topological_sort(): graph has a cycle");
    }
}
