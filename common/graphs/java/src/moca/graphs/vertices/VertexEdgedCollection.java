package moca.graphs.vertices;

import moca.graphs.edges.NeighbourEdge;
import java.util.Iterator;

public interface VertexEdgedCollection<V,E> extends VertexCollection<V> {

	int getNbEdges();
	void setEdgeValue(int idV,E value);
	E getEdgeValue(int idU, int idV);
	int getNeighbourAt(int idU, int index);
	void removeEdge(int idU, int idV);
	Iterator<NeighbourEdge<E> > neighbourIterator(int id);

};

