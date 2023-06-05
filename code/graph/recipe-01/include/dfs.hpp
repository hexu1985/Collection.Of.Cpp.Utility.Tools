#pragma once

#include <list>

#include "dfs_visit.hpp"

// depth-first search. dfsList contains all the graph vertices in the
// reverse order of their finishing times
template <typename T>
void dfs(Graph<T>& g, std::list<T>& dfsList)
{
	// clear dfsList
	dfsList.erase(dfsList.begin(), dfsList.end());

	// initialize all vertices to WHITE
	for (size_t i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = VertexInfo<T>::WHITE;

	// scan vInfo, calling () dfs_visit() for each WHITE vertex.
	for (size_t i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied && g.vInfo[i].color ==
											VertexInfo<T>::WHITE)
			dfs_visit(g,(*(g.vInfo[i].vtxMapLoc)).first, dfsList, false);
}

