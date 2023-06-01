
// input a graph
friend std::istream& operator>> (std::istream& istr, graph<T>& g)
{
	// nVertices is number of vertices to read
	int i, nVertices, nEdges;
	// use for input of vertices (v1) and edges ( {v1, v2} )
	T v1, v2;
	// edge weight
	int weight;

	if (g.numVertices > 0)
		// remove an existing graph
		g.clear();

	// input the number of vertices
	istr >> nVertices;

	// input the vertices and insert each into the graph
	for (i = 0; i < nVertices; i++)
	{
		istr >> v1;
		g.insertVertex(v1);
	}

	// input the number of edges
	istr >> nEdges;

	// input the vertices and weight for each edge, and
	// insert it into the graph
	for (i = 0; i < nEdges; i++)
	{
		istr >> v1;
		istr >> v2;
		istr >> weight;
		g.insertEdge(v1,v2,weight);
	}

	// return the stream
	return istr;
}

// output a graph
friend std::ostream& operator<< (std::ostream& ostr, const graph<T>& g)
{
	vertexInfo<T> vtxInfo;
    std::set<neighbor>::iterator setIter;

	int i;

	for (i = 0; i < g.vInfo.size(); i++)
	{
		vtxInfo = g.vInfo[i];
		if (vtxInfo.occupied)
		{
			ostr << (*(vtxInfo.vtxMapLoc)).first << ": in-degree " << vtxInfo.inDegree
				  << "  out-degree " << (vtxInfo.edges).size() << std::endl;
			ostr << "    Edges: ";
            std::set<neighbor>& edgeSet = vtxInfo.edges;
			for (setIter = edgeSet.begin(); setIter != edgeSet.end(); setIter++)
			{
				ostr << (*(g.vInfo[(*setIter).dest].vtxMapLoc)).first << " ("
					  << (*setIter).weight << ")  ";
			}
			ostr << std::endl;
		}
	}
	return ostr;
}


