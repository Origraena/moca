package moca.graphs.vertices;

import java.util.ArrayList;

public class ParentFunction<V> implements VertexBinaryFunction<V> {

	public ParentFunction(int nbVertices) {
		_parent = new ArrayList<Vertex<V> >(nbVertices);
		for (int i = 0 ; i < nbVertices ; i++)
			_parent.add(null);
	}

	public void exec(Vertex<V> current, Vertex<V> neighbour) {
		_parent.set(neighbour.getID(),current);
	}
	
	public ArrayList<Vertex<V> > getParent() {
		return _parent;
	}

	public int getNbVertices() {
		return _parent.size();
	}

	public Vertex<V> getParent(Vertex<V> vertex) {
		return _parent.get(vertex.getID());
	}

	public Vertex<V> getParent(int vertexID) {
		return _parent.get(vertexID);
	}

	private ArrayList<Vertex<V> > _parent = null;

};

