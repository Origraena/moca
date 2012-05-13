package moca.graphs.edges;

import java.lang.Iterable;
import java.util.NoSuchElementException;
import java.lang.UnsupportedOperationException;
import java.util.Iterator;

public interface EdgeCollection<Value> extends Iterable<Edge<Value> > {

	void onVertexAdded(int idV);

	void onVertexRemoved(int idV);

	void onVertexContracted(int idU, int idV);

	EdgeCollection<Value> subset(int idBegin, int idEnd);

	Edge<Value> get(int idU, int idV) throws NoSuchElementException;

	Edge<Value> get(int id) throws NoSuchElementException, UnsupportedOperationException;

	Value getValue(int idU, int idV) throws NoSuchElementException;

	int getNbNeighbours(int id) throws UnsupportedOperationException,NoSuchElementException;

	int getNeighbourAt(int idU, int index) throws UnsupportedOperationException, NoSuchElementException;

	void add(int idU, int idV, Value v) throws NoSuchElementException, IllegalEdgeException;

	void remove(int idU, int idV) throws NoSuchElementException;

	void remove (int id) throws NoSuchElementException, UnsupportedOperationException;

	int size();

	void clear();

	void add(Edge<Value> edge) throws NoSuchElementException, IllegalEdgeException;

	boolean contains(int idU, int idV);

	boolean contains(Edge<Value> edge);

	boolean remove(Edge<Value> edge);

	Iterator<Edge<Value> > iterator() throws UnsupportedOperationException;

	Iterator<NeighbourEdge<Value> > neighbourIterator(int id);

};

