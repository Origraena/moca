package moca.graphs.vertices;

import java.lang.Iterable;
import java.util.Iterator;
import java.lang.UnsupportedOperationException;

public interface VertexCollection<Value> extends Iterable<Vertex<Value> > {

	void clear();

	int size();

	VertexCollection<Value> subset(int idBegin, int idEnd);

	boolean add(Vertex<Value> v);

	Vertex<Value> remove(int id) throws UnsupportedOperationException;

	Vertex<Value> get(int id);

	// this method should not be used directly, it is used as a wrapper for the class graph
	Vertex<Value> set(int id, Vertex<Value> v);

	Iterator<Vertex<Value> > iterator();

};

