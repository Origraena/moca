package moca.graphs;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexCollection;
import moca.graphs.vertices.BipartedVertexCollection;
import moca.graphs.vertices.VertexBinaryFunction;
import moca.graphs.vertices.ParentFunction;
import moca.graphs.edges.Edge;
import moca.graphs.edges.NeighbourEdge;
import moca.graphs.edges.EdgeCollection;
import moca.graphs.edges.IllegalEdgeException;

import java.util.Iterator;

public class BipartedGraph<V,E> extends Graph<V,E> {

	public BipartedGraph(BipartedVertexCollection<V> vertices, EdgeCollection<E> edges) throws IllegalConstructionException {
		super(vertices,edges);
		if (vertices.size() > 0) {
			int i = 0;
			for (Vertex<V> v : vertices) {
				v.setID(i);
				i++;
			}
		}
	}

	public BipartedGraph(VertexCollection<V> firstSet, VertexCollection<V> secondSet, EdgeCollection<E> edges) throws IllegalConstructionException {
		super(new BipartedVertexCollection<V>(firstSet,secondSet), edges);
		if (_vertices.size() > 0) {
			int i = 0;
			for (Vertex<V> v : _vertices) {
				v.setID(i);
				i++;
			}
		}
	}

	public BipartedGraph(Graph<V,E> g) throws IllegalConstructionException {
		super(g);
	}

	public void addEdge(int idU, int idV, E value) throws IllegalEdgeException {
		if (((idU >= bipartedVertices().firstSet().size()) && (idV >= bipartedVertices().firstSet().size()))
		||  ((idU < bipartedVertices().firstSet().size()) && (idV < bipartedVertices().firstSet().size())))
			throw new IllegalEdgeException();
		super.addEdge(idU,idV,value);
	}

	public Vertex<V> getInFirstSet(int id) throws NoSuchElementException {
		return _firstSet.get(id);
	}

	public void addInFirstSet(V value) {
		addVertexInFirstSet(new Vertex<V>(bipartedVertices().firstSet().size(), value));
	}

	public void addInSecondSet(V value) {
		addVertexInSecondSet(new Vertex<V>(bipartedVertices().size(), value));
	}

	public void addVertexInFirstSet(Vertex<V> v) {
		v.setID(bipartedVertices().firstSet().size());
		bipartedVertices().addInFirstSet(v);
		_edges.onVertexAdded(v.getID());
	}

	public void addVertexInSecondSet(Vertex<V> v) {
		v.setID(bipartedVertices().size());
		bipartedVertices().addInSecondSet(v);
		_edges.onVertexAdded(v.getID());
	}

	public Iterator<Vertex<V> > firstVertexIterator() {
		return bipartedVertices().firstSet().iterator();
	}

	public Iterator<Vertex<V> > secondVertexIterator() {
		return bipartedVertices().secondSet().iterator();
	}

	public Iterator<V> firstIterator() {
		return new Graph<V,E>.VertexValueIterator(firstVertexIterator());
	}

	public Iterator<V> secondIterator() {
		return new Graph<V,E>.VertexValueIterator(secondVertexIterator());
	}

	protected BipartedVertexCollection<V> bipartedVertices() {
		return (BipartedVertexCollection<V>) _vertices;
	}

};

