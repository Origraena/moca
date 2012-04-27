package moca.graphs.vertices;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class VertexArrayList<V> extends ArrayList<Vertex<V> > implements VertexCollection<V> {

	private static final long serialVersionUID = 2012022600L; 

	public VertexArrayList<V> subset(int idBegin, int idEnd) {
		VertexArrayList<V> result = new VertexArrayList<V>();
		for (int i = idBegin ; i < idEnd ; i++)
			result.add(get(i).clone());
		return result;
	}

	public boolean add(Vertex<V> v) {
		//v.setID(size());
		return super.add(v);
	}

	public Vertex<V> remove(int id) {
		Vertex<V> v = super.remove(id);
		for (int i = id ; i < size() ; i++)
			get(i).setID(i);
		return v;
	}

};

