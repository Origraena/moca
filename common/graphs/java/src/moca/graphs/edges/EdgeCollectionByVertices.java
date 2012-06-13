package moca.graphs.edges;

import moca.graphs.vertices.VertexEdgedCollection;

import java.lang.Iterable;
import java.util.NoSuchElementException;
import java.lang.UnsupportedOperationException;
import java.util.Iterator;

public class EdgeCollectionByVertices<E> extends AbstractEdgeCollection<E> {

	public EdgeCollectionByVertices(VertexEdgedCollection<?,E> vertices) {
		_vertices = vertices;
	}

	public void setNbVertices(int nb) {
//	TODO
	}

	public void onVertexAdded(int idV) {
//		_edgeValues.add(null);
	}

	public void onVertexRemoved(int idV) {
//		_edgeValues.remove(idV);
	}

	public void onVertexContracted(int idU, int idV) {
//		_edgeValues.set(idU,_edgeValues.get(idV));
	}

	public EdgeCollection<E> subset(int idBegin, int idEnd) {
		return this;
	}

	public Edge<E> get(int id) throws NoSuchElementException, UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}

	public E getValue(int idU, int idV) throws NoSuchElementException {
		return _vertices.getEdgeValue(idU,idV);
	}

	public int getNbNeighbours(int id) throws UnsupportedOperationException,NoSuchElementException {
		return _vertices.getNbNeighbours(id);
	}

	public int getNeighbourAt(int idU, int index) throws UnsupportedOperationException, NoSuchElementException {
		return _vertices.getNeighbourAt(idU,index);
	}

	public void add(int idU, int idV, E v) throws NoSuchElementException, IllegalEdgeException { }

	public void remove(int idU, int idV) throws NoSuchElementException {
		_vertices.removeEdge(idU,idV);
	}

	public int size() {
		return _vertices.getNbEdges();
	}

	public void clear() { }

	public Iterator<NeighbourEdge<E> > neighbourIterator(int id) {
		return _vertices.neighbourIterator(id);
	}

	private VertexEdgedCollection<?,E> _vertices;

};

