#pragma once

#include <set>
#include <vector>
#include <list>

// depth-first visit assuming a WHITE starting vertex. dfsList
// contains the visited vertices in reverse order of finishing time.
// when checkForCycle is true, the function throws an exception if
// it detects a cycle
template <typename T>
void dfs_visit(Graph<T>& g, const T& sVertex, std::list<T>& dfsList,
        bool checkForCycle)
{
    // indices for vertex positions in vInfo
    int pos_sVertex, pos_neighbor;

    // iterator to scan the adjacency set of a vertex
    std::set<Neighbor>::iterator adj;

    // alias to simplify access to the vector vInfo
    std::vector<VertexInfo<T>>& vlist = g.vInfo;

    // fetch the index for sVertex in vInfo; throw an exception
    // if the starting vertex is not in the Graph
    pos_sVertex = g.GetVertexInfoIndex(sVertex);

    if (pos_sVertex == -1)
        throw GraphError("graph dfs_visit(): vertex not in the graph");

    // color vertex GRAY to note its discovery
    vlist[pos_sVertex].color = VertexInfo<T>::GRAY;

    // create an alias for the adjacency set of sVertex
    std::set<Neighbor>& edgeSet = vlist[pos_sVertex].edges;

    // sequence through the adjacency set and look for vertices
    // that are not yet discovered (colored WHITE). recursively call
    // dfs_visit() for each such vertex. if a vertex in the adjacency
    // set is GRAY, the vertex was discovered during a previous
    // call and there is a cycle that begins and ends at the
    // vertex; if checkForCycle is true, throw an exception
    for (adj = edgeSet.begin(); adj != edgeSet.end(); adj++)
    {
        pos_neighbor = (*adj).dest;
        if (vlist[pos_neighbor].color == VertexInfo<T>::WHITE)
            dfs_visit(g,(*(g.vInfo[pos_neighbor].vtxMapLoc)).first,
                    dfsList, checkForCycle);
        else if (vlist[pos_neighbor].color == VertexInfo<T>::GRAY
                && checkForCycle)
            throw GraphError("graph dfs_visit(): graph has a cycle");
    }

    // finished with vertex sVertex. make it BLACK and add it to
    // the front of dfsList
    vlist[pos_sVertex].color = VertexInfo<T>::BLACK;
    dfsList.push_front((*(g.vInfo[pos_sVertex].vtxMapLoc)).first);
}
