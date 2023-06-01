#ifdef __BORLANDC__
// suppress the warning message that functions containing for are not
// expanded inline
#pragma warn -8027
#endif	// __BORLANDC__


// input a graph
friend istream& operator>> (istream& istr, graph<T>& g)
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
friend ostream& operator<< (ostream& ostr, const graph<T>& g)
{
	vertexInfo<T> vtxInfo;
	set<neighbor>::iterator setIter;

	int i;

	for (i = 0; i < g.vInfo.size(); i++)
	{
		vtxInfo = g.vInfo[i];
		if (vtxInfo.occupied)
		{
			ostr << (*(vtxInfo.vtxMapLoc)).first << ": in-degree " << vtxInfo.inDegree
				  << "  out-degree " << (vtxInfo.edges).size() << endl;
			ostr << "    Edges: ";
			set<neighbor>& edgeSet = vtxInfo.edges;
			for (setIter = edgeSet.begin(); setIter != edgeSet.end(); setIter++)
			{
				ostr << (*(g.vInfo[(*setIter).dest].vtxMapLoc)).first << " ("
					  << (*setIter).weight << ")  ";
			}
			ostr << endl;
		}
	}
	return ostr;
}

// perform the breadth-first traversal from sVertex and
// return the set of visited vertices
friend set<T> bfs(graph<T>& g, const T& sVertex)
{
	// BFS uses a queue to store indices of adjacent vertices from vInfo
   queue<int> visitQueue;

	// set of vertices in BFS
	set<T> visitSet;

	// use to store indices in vInfo
	int currVertex, neighborVertex;

	// use to search edge sets for unvisited vertices
	set<neighbor>::iterator adj;
	int i;

	// find the index of the starting vertex
	currVertex = g.getvInfoIndex(sVertex);

	// check for a nonexistent starting vertex
	if (currVertex == -1)
		throw graphError("graph bfs(): vertex not in the graph");

	// initialize all vertices in the graph to unvisited (WHITE)
	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

   visitQueue.push(currVertex);   // initialize the queue

   while (!visitQueue.empty())
   {
      // remove a vertex from the queue
      currVertex = visitQueue.front();
		visitQueue.pop();
		// indicate that the vertex has been visited
		g.vInfo[currVertex].color = vertexInfo<T>::BLACK;

		// put the vertex in visitSet
		visitSet.insert((*(g.vInfo[currVertex].vtxMapLoc)).first);

		// create an alias for the edge set of currVertex
		set<neighbor>& edgeSet = g.vInfo[currVertex].edges;
		// sequence through the edge set and look for vertices
		// that have not been visited
		for (adj = edgeSet.begin(); adj != edgeSet.end(); adj++)
		{
			neighborVertex = (*adj).dest;

			if (g.vInfo[neighborVertex].color == vertexInfo<T>::WHITE)
			{
				g.vInfo[neighborVertex].color = vertexInfo<T>::GRAY;
				visitQueue.push(neighborVertex);
			}
		}
   }

	return visitSet;
}

// use the breadth-first traversal algorithm to determine the
// minimum number of edges in any path from sVertex to eVertex
// or -1 if no path exists. if a path exists, the list path
// is the sequence of vertices
friend int shortestPath(graph<T>& g, const T& sVertex,
			               const T& eVertex, list<T>& path)
{
   // queue stores vertices as vInfo indices
   queue<int>  visitQueue;

   // eIter scans the vertices in an adjacency set
   set<neighbor>::iterator eIter;

	// flag set true when scan identifies eVertex as a neighbor
	bool foundShortestPath = false;

	// index in vInfo for the source and destination vertices
	// and the index for current vertex and a neighbor
	int pos_sVertex, pos_eVertex, currPos, neighborPos;

	int returnValue;

	// initialize all vertices to undiscoved (WHITE)
	for (int i = 0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

	// obtain the starting and ending indices
	pos_sVertex = g.getvInfoIndex(sVertex);
	pos_eVertex = g.getvInfoIndex(eVertex);

	if (pos_sVertex == -1 || pos_eVertex == -1)
		throw graphError("graph shortestPath(): vertex not in "
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
		g.vInfo[currPos].color = vertexInfo<T>::BLACK;

		// if we are at eVertex, we have found the shortest
		// path from sVertex to eVertex
		if (currPos == pos_eVertex)
			foundShortestPath = true;
		else
		{
			// create an alias for the adjacency set of currPos
			set<neighbor>& edgeSet = g.vInfo[currPos].edges;

			// for all undiscovered neighbors, update the dataValue,
			// color, and parent fields in the vertexInfo object.
			for (eIter = edgeSet.begin(); eIter != edgeSet.end(); eIter++)
			{
				neighborPos = (*eIter).dest;

				if (g.vInfo[neighborPos].color == vertexInfo<T>::WHITE)
				{
 					g.vInfo[neighborPos].dataValue =
										g.vInfo[currPos].dataValue + 1;
					g.vInfo[neighborPos].parent = currPos;
					g.vInfo[neighborPos].color = vertexInfo<T>::GRAY;
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

// find the path with minimum total weight from sVertex to eVertex
// and return the minimum weight
friend int minimumPath(graph<T>& g, const T& sVertex, const T& eVertex,
							  list<T>& path)
{
   // heap (priority queue) that stores minInfo objects
   miniPQ<minInfo,less<minInfo> >  minPathPQ;

   // used when inserting minInfo entries
   // into the priority queue or erasing entries
   minInfo vertexData;
   // adj is edge set of vertex we are visiting. adjiter is used
   // to traverse adj
   set<neighbor> adj;
   set<neighbor>::iterator adjIter;

	bool foundMinPath = false;

	// index in vInfo for the starting and ending vertices
	// position in vInfo of vertex on a path from sVertex
	int pos_sVertex, pos_eVertex, currPos, destPos;

	// computed minimum weight
   int newMinWeight;

	// return value
	int returnValue;

	typename list<T>::iterator pathIter;

	// initialize all vertices to unmarked (WHITE) and dataValue
	// variables to INFINITY
	for (int i = 0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
		{
			g.vInfo[i].color = vertexInfo<T>::WHITE;
			g.vInfo[i].dataValue = INFINITY;
		}

	// obtain the starting and ending indices
	pos_sVertex = g.getvInfoIndex(sVertex);
	pos_eVertex = g.getvInfoIndex(eVertex);

	if (pos_sVertex == -1 || pos_eVertex == -1)
		throw graphError("graph minimumPath(): vertex not in the graph");


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

		if (g.vInfo[currPos].color != vertexInfo<T>::BLACK)
		{
			// mark the vertex so we don't look at it again
			g.vInfo[currPos].color = vertexInfo<T>::BLACK;

			// find all neighbors of the current vertex pos. for each
			// neighbor that has not been visited, generate a minInfo
			// object and insert it into the priority queue provided the
			// total weight to get to the neighbor is better than the
			// current dataValue in vInfo
			adj = g.vInfo[currPos].edges;
			for(adjIter = adj.begin();adjIter != adj.end();
										adjIter++)
			{
				destPos = (*adjIter).dest;

				if (g.vInfo[destPos].color == vertexInfo<T>::WHITE)
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

// find the minimum spanning tree for the strongly connected digraph g
friend int minSpanTree(graph<T>& g, graph<T>& MST)
{
   // priority queue that stores minInfo objects
   miniPQ<minInfo, less<minInfo> >  minTreePQ;

   // used when inserting minInfo entries
   // into the priority queue or erasing entries
   minInfo vertexData;

   // adj is adjacency set of vertex we are visiting. adjIter
	// is an iterator that scans the list
	set<neighbor> adj;
   set<neighbor>::iterator adjIter;

	// size of the minimum spanning tree
	int minSpanTreeSize = 0, i;

	// index of a starting vertex.
	int pos_sVertex = -1, currPos, destPos, parentPos;

	// current minimum total weight for spanning tree
   int minSpanTreeWeight = 0;

	// initialize all vertices unmarked and their dataValue fields
	// to infinity;  select the first vertex as the	starting vertex;
	for (i = 0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
		{
			if (pos_sVertex == -1)
				pos_sVertex = i;
			g.vInfo[i].color = vertexInfo<T>::WHITE;
			g.vInfo[i].dataValue = INFINITY;
		}

   // create minInfo object for starting vertex
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
		if (g.vInfo[currPos].color == vertexInfo<T>::WHITE)
		{
			minSpanTreeWeight += vertexData.pathWeight;
			minSpanTreeSize++;

			// if we spanned all vertices, break
			if (minSpanTreeSize == g.numberOfVertices())
				break;

			// mark the vertex BLACK so we don't look at it again.
			// set dataValue in vInfo vector to current
			// minimum tree weight
			g.vInfo[currPos].color = vertexInfo<T>::BLACK;

			// find all unmarked neighbors of the vertex.
			// adjIter is a set iterator pointing at the edge corresponding to
			// vertices with index currPos and destPos
			adj = g.vInfo[currPos].edges;
			for(adjIter = adj.begin();adjIter != adj.end();	adjIter++)
			{
				destPos = (*adjIter).dest;

				// if neighbor is unmarked, check whether adding the new
				// edge to the tree is better than using the current edge
				if (g.vInfo[destPos].color == vertexInfo<T>::WHITE)
				{
					if ((*adjIter).weight < g.vInfo[destPos].dataValue)
					{
						// if new edge is a better connection, create minInfo object
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

	for(i = 0; i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			MST.insertVertex((*(g.vInfo[i]).vtxMapLoc).first);

	// add the edges to the minimum spanning tree
	for (i = pos_sVertex+1; i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
		{
			parentPos = g.vInfo[i].parent;
			vA = (*(g.vInfo[parentPos]).vtxMapLoc).first;
			vB = (*(g.vInfo[i]).vtxMapLoc).first;
			MST.insertEdge(vA,vB, g.getWeight(vA,vB));
		}

   return minSpanTreeWeight;
}

// determine if the graph is acyclic
friend bool acyclic(graph<T>& g)
{
	int i;
	// use for calls to dfsVisit()
	list<T> dfsList;

	// initialize all vertices to WHITE
	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

	// scan vInfo, calling dfsVisit() for each WHITE vertex.
	// catch a graphError exception in a call to dfsVisit()
	try
	{
		for (i=0;i < g.vInfo.size(); i++)
			if (g.vInfo[i].occupied && g.vInfo[i].color ==
												vertexInfo<T>::WHITE)
					dfsVisit(g,(*(g.vInfo[i].vtxMapLoc)).first,
									 dfsList, true);
	}

	catch (const graphError&)
	{
		return false;
	}

	return true;
}

// this algorithm is a private friend of the graph class
private:

// depth-first visit assuming a WHITE starting vertex. dfsList
// contains the visited vertices in reverse order of finishing time.
// when checkForCycle is true, the function throws an exception if
// it detects a cycle
friend void dfsVisit(graph<T>& g, const T& sVertex, list<T>& dfsList,
							bool checkForCycle)
{
	// indices for vertex positions in vInfo
	int pos_sVertex, pos_neighbor;

	// iterator to scan the adjacency set of a vertex
	set<neighbor>::iterator adj;

	// alias to simplify access to the vector vInfo
	vector<vertexInfo<T> >& vlist = g.vInfo;

	// fetch the index for sVertex in vInfo; throw an exception
	// if the starting vertex is not in the graph
	pos_sVertex = g.getvInfoIndex(sVertex);

	if (pos_sVertex == -1)
		throw graphError("graph dfsVisit(): vertex not in the graph");

	// color vertex GRAY to note its discovery
	vlist[pos_sVertex].color = vertexInfo<T>::GRAY;

	// create an alias for the adjacency set of sVertex
	set<neighbor>& edgeSet = vlist[pos_sVertex].edges;

	// sequence through the adjacency set and look for vertices
	// that are not yet discovered (colored WHITE). recursively call
	// dfsVisit() for each such vertex. if a vertex in the adjacency
	// set is GRAY, the vertex was discovered during a previous
	// call and there is a cycle that begins and ends at the
	// vertex; if checkForCycle is true, throw an exception
	for (adj = edgeSet.begin(); adj != edgeSet.end(); adj++)
	{
		pos_neighbor = (*adj).dest;
		if (vlist[pos_neighbor].color == vertexInfo<T>::WHITE)
			dfsVisit(g,(*(g.vInfo[pos_neighbor].vtxMapLoc)).first,
							 dfsList, checkForCycle);
		else if (vlist[pos_neighbor].color == vertexInfo<T>::GRAY
					&& checkForCycle)
			throw graphError("graph dfsVisit(): graph has a cycle");
	}

	// finished with vertex sVertex. make it BLACK and add it to
	// the front of dfsList
	vlist[pos_sVertex].color = vertexInfo<T>::BLACK;
	dfsList.push_front((*(g.vInfo[pos_sVertex].vtxMapLoc)).first);
}

// return to the public section for the remaining algorithms
public:

// depth-first search. dfsList contains all the graph vertices in the
// reverse order of their finishing times
friend void dfs(graph<T>& g, list<T>& dfsList)
{
	int i;

	// clear dfsList
	dfsList.erase(dfsList.begin(), dfsList.end());

	// initialize all vertices to WHITE
	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

	// scan vInfo, calling () dfsVisit() for each WHITE vertex.
	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied && g.vInfo[i].color ==
											vertexInfo<T>::WHITE)
			dfsVisit(g,(*(g.vInfo[i].vtxMapLoc)).first, dfsList, false);
}

// find a topological sort of an acyclic graph
friend void topologicalSort(graph<T>& g, list<T>& tlist)
{
	int i;

	// clear the list that will contain the sort
	tlist.erase(tlist.begin(), tlist.end());

	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

	// cycle through the vertices, calling dfsVisit() for each
	// WHITE vertex. check for a cycle
	try
	{
		for (i=0;i < g.vInfo.size(); i++)
			if (g.vInfo[i].occupied && g.vInfo[i].color == vertexInfo<T>::WHITE)
				dfsVisit(g, (*(g.vInfo[i].vtxMapLoc)).first, tlist, true);
	}

	catch(graphError&)
	{
		throw graphError("graph topologicalSort(): graph has a cycle");
	}
}

// return the transpose of the graph
friend graph<T> transpose(graph<T>& g)
{
	int i, n = g.vInfo.size();
	set<neighbor>::iterator setIter;
	// initialize gt as a copy of g
	graph<T> gt = g;

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
			set<neighbor>& s = g.vInfo[i].edges;

			// edge from index i to index dest in vInfo for g must be an edge
			// from dest to i with the same weight in gt. increment the in-degree
			// at index i
			for (setIter = s.begin(); setIter != s.end(); setIter++)
			{
				gt.vInfo[(*setIter).dest].edges.insert(neighbor(i,(*setIter).weight));
				gt.vInfo[i].inDegree++;
			}
		}

	return gt;
}

// find the strong components of the graph
friend void strongComponents(graph<T>& g, vector<set<T> >& component)
{
	// list of vertices visited by dfs() for graph g
	list<T> dfsGList;

	// list of vertices visited by dfsVisit() for g transpose
	list<T> dfsGTList;

	// used to scan dfsGList and dfsGTList objects
	typename list<T>::iterator gIter, gtIter;

	// transpose of the graph
	graph<T> gt;

	// set for an individual strong component
	set<T> scSet;

	int i;

	// clear the return vector
	component.resize(0);

	// execute depth-first tranversal of g
	dfs(g, dfsGList);

	// compute gt
	gt = transpose(g);

	// initialize all vertices in gt to WHITE (unvisited)
	for (i=0;i < gt.vInfo.size(); i++)
		if (gt.vInfo[i].occupied)
			gt.vInfo[i].color = vertexInfo<T>::WHITE;

	// call dfsVisit() for gt from vertices in dfsGList
	gIter = dfsGList.begin();
	while(gIter != dfsGList.end())
	{
		// call dfsVisit() only if vertex has not been visited
		if (gt.vInfo[gt.getvInfoIndex(*gIter)].color ==
						 vertexInfo<T>::WHITE)
		{
			// clear dfsGTList and scSet
			dfsGTList.erase(dfsGTList.begin(), dfsGTList.end());
			scSet.erase(scSet.begin(), scSet.end());

			// do dfsVisit() in gt for starting vertex *gIter
			dfsVisit(gt, *gIter, dfsGTList, false);

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
