package moca.graphs.vertices;

import java.lang.Iterable;
import java.util.Iterator;
import java.lang.UnsupportedOperationException;

public interface VertexCollection<Value> extends Iterable<Vertex<Value> > {

	void clear();

	int size();

	boolean add(Vertex<Value> v);

	Vertex<Value> remove(int id) throws UnsupportedOperationException;

	Vertex<Value> get(int id);

	Iterator<Vertex<Value> > iterator();

};

