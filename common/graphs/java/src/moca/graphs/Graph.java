package moca.graphs;

import moca.operators.OperatorPlus1T;

import moca.lists.LinkedList;
import moca.lists.Lifo;
import moca.lists.Fifo;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexCollection;
import moca.graphs.vertices.VertexBinaryFunction;
import moca.graphs.vertices.ParentFunction;
import moca.graphs.edges.Edge;
import moca.graphs.edges.NeighbourEdge;
import moca.graphs.edges.EdgeCollection;
import moca.graphs.edges.IllegalEdgeException;


import java.util.Collection;
import java.lang.Iterable;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.lang.UnsupportedOperationException;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Comparator;



public class Graph<V,E> implements Iterable<V> {

	public Graph(VertexCollection<V> vertices, EdgeCollection<E> edges) throws IllegalConstructionException {
		if ((vertices == null) || (edges == null))
			throw new IllegalConstructionException();
		_vertices = vertices;
		_edges = edges;
	}

	/**
	 * The graph will be cloned but not vertices values.
	 * An IllegalConstructionException is thrown whenever an incompatible graph is attempted to be copied.
	 */
	public Graph(Graph<V,E> g) throws IllegalConstructionException {
		try {
			for (V v : g)
				addVertex(v/*.clone()*/);
			for (Iterator<Edge<E> > e = g.edgeIterator() ; e.hasNext() ; )
				addEdge(e.next());
		}
		catch (IllegalEdgeException e) {
			throw new IllegalConstructionException();
		}
	}

	public int getNbVertices() {
		return _vertices.size();
	}

	public int getNbEdges() {
		return _edges.size();
	}

	public Vertex<V> getVertex(int id) throws NoSuchElementException {
		return _vertices.get(id);
	}

	public V get(int id) throws NoSuchElementException {
		return getVertex(id).getValue();
	}

	public void addVertex(V value) {
		Vertex<V> v = new Vertex<V>(value);
		_vertices.add(v);					// this method should change vertex id to match its index
		_edges.onVertexAdded(v.getID());	// uses directly vertex class ?
	}

	public void removeVertex(int id) {
		_edges.onVertexRemoved(id);
		_vertices.remove(id);
	}

	public Edge<E> getEdge(int idU, int idV) throws NoSuchElementException {
		return _edges.get(idU,idV);
	}

	public Edge<E> getEdge(Vertex<V> u, Vertex<V> v) throws NoSuchElementException {
		return _edges.get(u.getID(),v.getID());
	}

	public void addEdge(int idU, int idV, E value) throws NoSuchElementException, IllegalEdgeException {
		if ((idU >= getNbVertices()) || (idV >= getNbVertices()))
			throw new NoSuchElementException();
		_edges.add(idU, idV, value);
	}

	public void addEdge(Edge<E> edge) throws NoSuchElementException, IllegalEdgeException {
		if ((edge.getIDU() >= getNbVertices()) || (edge.getIDV() >= getNbVertices()))
			throw new NoSuchElementException();
		_edges.add(edge);
	}

	public void addEdge(Vertex<V> u, Vertex<V> v, E value) throws NoSuchElementException, IllegalEdgeException {
		_edges.add(u.getID(),v.getID(),value);
	}

	public void removeEdge(int idU, int idV) throws NoSuchElementException {
		if ((idU >= getNbVertices()) || (idV >= getNbVertices()))
			throw new NoSuchElementException();
		_edges.remove(idU,idV);
	}

	public void removeEdge(Vertex<V> u, Vertex<V> v) throws NoSuchElementException {
		_edges.remove(u.getID(),v.getID());
	}

	public Iterator<V> iterator() {
		return new VertexValueIterator(_vertices.iterator());
	}

	public Iterator<Vertex<V> > vertexIterator() {
		return _vertices.iterator();
	}

	public Iterator<Edge<E> > edgeIterator() {
		return _edges.iterator();
	}

	public Iterator<NeighbourEdge<E> > neighbourIterator(int id) throws NoSuchElementException {
		if (id > getNbVertices())
			throw new NoSuchElementException();
		return _edges.neighbourIterator(id);
	}

	public WalkIterator BFSIterator() {
		return new BFSIterator(this,0,null);
	}

	public WalkIterator BFSIterator(int root) {
		return new BFSIterator(this,root,null);
	}

	public WalkIterator BFSIterator(int root, VertexBinaryFunction<V> function) {
		return new BFSIterator(this,root,function);
	}

	public WalkIterator DFSIterator() {
		System.out.println("test");
		return new DFSIterator(this,0,null);
	}

	public WalkIterator DFSIterator(int root) {
		return new DFSIterator(this,root,null);
	}

	public WalkIterator DFSIterator(int root, VertexBinaryFunction<V> function) {
		return new DFSIterator(this,root,function);
	}

	public ParentFunction<V> Dijsktra(int root, E zeroValue, OperatorPlus1T<E> plus, Comparator<Vertex<V> > compareVertex, Comparator<E> compareEdge) {
		return AStar(root,
					 zeroValue,
					 plus,
					 compareVertex,
					 compareEdge,
					 null);
	}

	public ParentFunction<V> AStar(int root, 
								   E zeroValue,
								   OperatorPlus1T<E> plus,
								   Comparator<Vertex<V> > compareVertex,
								   Comparator<E> compareEdge,
//								   ArrayList<Vertex<V> > ends, 
								   VertexBinaryFunction<V> heuristique) {
		ArrayList<E> weights = new ArrayList<E>(getNbVertices());	// contains the total weights of edges between root and index vertices
		for (int i = 0 ; i < getNbVertices() ; i++)
			weights.add(null);
		weights.set(root,zeroValue);
		Vertex<V> u = null;
		NeighbourEdge<E> e = null;
		PriorityQueue<Vertex<V> > queue = new PriorityQueue<Vertex<V> >(11,compareVertex);
		ParentFunction<V> parent = new ParentFunction<V>(getNbVertices());
		u = getVertex(root);
		while (u != null) {
//			if (ends.contains(u))
//				return parent;
			for (Iterator<NeighbourEdge<E> > it = neighbourIterator(u.getID()) ; it.hasNext() ; ) {
				e = it.next();
				if ((weights.get(e.getIDV()) == null) || 
					(compareEdge.compare(plus.exec(weights.get(u.getID()),e.getValue()),
								  weights.get(e.getIDV())) < 0)) {
					queue.remove(getVertex(e.getIDV()));
					weights.set(e.getIDV(),plus.exec(weights.get(u.getID()),e.getValue()));
					parent.exec(u,getVertex(e.getIDV()));
					queue.add(getVertex(e.getIDV()));
				}
			}
			u = queue.poll();
		}
		return parent;
	}

	protected VertexCollection<V> _vertices = null;
	protected EdgeCollection<E> _edges = null;


	protected class VertexValueIterator implements Iterator<V> {
		
		public VertexValueIterator(Iterator<Vertex<V> > iterator) {
			_iterator = iterator;
		}

		public boolean hasNext() {
			return _iterator.hasNext();
		}

		public V next() {
			return _iterator.next().getValue();
		}

		public void remove() throws UnsupportedOperationException {
			_iterator.remove();
		}

		private Iterator<Vertex<V> > _iterator;

	}

	public class WalkIterator implements Iterator<Vertex<V> > {

		protected WalkIterator(Graph<V,E> source, int rootID, VertexBinaryFunction<V> function) {
			if (source.getNbVertices() == 0)
				_current = null;
			else {
				_source = source;
				_current = source.getVertex(rootID);
				_function = function;
				_colors = new int[source.getNbVertices()];
				_neighbourIterators = new ArrayList<Iterator<NeighbourEdge<E> > >(source.getNbVertices());
				for (int i = 0 ; i < source.getNbVertices() ; i++) {
					_neighbourIterators.add(_source.neighbourIterator(i));
					_colors[i] = 0;
				}
			}
		}

		protected WalkIterator(Graph<V,E> source, Vertex<V> root, VertexBinaryFunction<V> function) {
			_source = source;
			_current = root;
			_function = function;
			_colors = new int[source.getNbVertices()];
			_neighbourIterators = new ArrayList<Iterator<NeighbourEdge<E> > >(source.getNbVertices());
			for (int i = 0 ; i < source.getNbVertices() ; i++) {
				_neighbourIterators.add(_source.neighbourIterator(i));
				_colors[i] = 0;
			}
		}

		public VertexBinaryFunction<V> getFunction() {
			return _function;
		}

		public Vertex<V> getCurrent() throws NoSuchElementException {
			if (_current == null)
				throw new NoSuchElementException();
			return _current;
		}

		public Vertex<V> processNext() {
			if (!hasNext())
				throw new NoSuchElementException();
			Vertex<V> head = _waitingList.get();
			Vertex<V> neighbour = null;
			NeighbourEdge<E> edge = null;
			if (_neighbourIterators.get(head.getID()).hasNext()) {
				edge = _neighbourIterators.get(head.getID()).next();
				neighbour = _source.getVertex(edge.getIDV());
				if (_colors[neighbour.getID()] == 0) {
					_colors[neighbour.getID()] = 1;
					_waitingList.put(neighbour);
					if (_function != null)
						_function.exec(head,neighbour);
					return neighbour;
				}
				else
					return processNext();
			}
			else {
				_waitingList.pop();
				return processNext();
			}
		}

		public Vertex<V> next() throws NoSuchElementException {
			if (!hasNext())
				throw new NoSuchElementException();
			Vertex<V> old = _current;
			try {_current = processNext();}
			catch (NoSuchElementException e) { _current = null; }	// may only be thrown by LinkedList.get()
			return old;
		}

		public boolean hasNext() {
			return (_current != null);
		}

		public void remove() throws UnsupportedOperationException {
			throw new UnsupportedOperationException();
		}

		protected Graph<V,E> _source = null;
		protected Vertex<V> _current = null;
		protected VertexBinaryFunction<V> _function = null;
		protected LinkedList<Vertex<V> > _waitingList = null;
		protected int _colors[];
		protected ArrayList<Iterator<NeighbourEdge<E> > > _neighbourIterators = null;

	};

	protected class BFSIterator extends WalkIterator {
		public BFSIterator(Graph<V,E> source, int root, VertexBinaryFunction<V> function) {
			super(source,root,function);
			_waitingList = new Fifo<Vertex<V> >();
			_waitingList.put(_current);
		}
	};

	protected class DFSIterator extends WalkIterator {
		public DFSIterator(Graph<V,E> source, int root, VertexBinaryFunction<V> function) {
			super(source,root,function);
			_waitingList = new Lifo<Vertex<V> >();
			_waitingList.put(_current);
		}
	};

};

