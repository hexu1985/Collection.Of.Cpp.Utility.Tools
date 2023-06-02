#pragma once

#include "dfs_visit.hpp"
#include <list>

// determine if the graph is acyclic
template <typename T>
bool acyclic(Graph<T>& g)
{
    // use for calls to dfs_visit()
    std::list<T> dfsList;

    // initialize all vertices to WHITE
    for (size_t i=0;i < g.vInfo.size(); i++)
        if (g.vInfo[i].occupied)
            g.vInfo[i].color = VertexInfo<T>::WHITE;

    // scan vInfo, calling dfs_visit() for each WHITE vertex.
    // catch a graphError exception in a call to dfs_visit()
    try
    {
        for (size_t i=0;i < g.vInfo.size(); i++)
            if (g.vInfo[i].occupied && g.vInfo[i].color ==
                    VertexInfo<T>::WHITE)
                dfs_visit(g,(*(g.vInfo[i].vtxMapLoc)).first,
                        dfsList, true);
    }
    catch (const GraphError&)
    {
        return false;
    }

    return true;
}
