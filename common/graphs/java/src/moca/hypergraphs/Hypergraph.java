package moca.hypergraphs;

public class HyperGraph<V,E> extends AbstractGraph<V,E> {

	public HyperGraph(VertexCollection<V> vertices, HyperEdgeCollection<E> edges) {
		super(vertices);
		_edges = edges;
	}
	
	public void clear() {
		super.clear();
		_edges.clear();
	}

	public int getNbEdges() {
		return _edges.size();
	}	
	
	public void removeVertex(int id) {
		_edges.onVertexRemoved(id);
		super.removeVertex(id);
	}
	
	public void addVertex(V value) {
		super.addVertex(value);
		_edges.onVertexAdded(v.getID());
	}
	
	private HyperEdgeCollection<E> _edges;

}

