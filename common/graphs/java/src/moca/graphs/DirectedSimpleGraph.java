package moca.graphs;

import moca.graphs.vertices.VertexArrayList;
import moca.graphs.edges.NeighboursLists;

import java.util.Iterator;

public class DirectedSimpleGraph<V,E> extends Graph<V,E> {

	public DirectedSimpleGraph() throws IllegalConstructionException {
		super(new VertexArrayList<V>(),new NeighboursLists<E>());
	}

	public DirectedSimpleGraph(Graph<V,E> g) throws IllegalConstructionException {
		super(g);
	}

};

