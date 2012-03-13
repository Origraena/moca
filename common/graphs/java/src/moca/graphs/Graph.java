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


/**
 * Generic graph class.
 * It is instantiated by giving two parameters : the vertex collection (empty if possible), and the edge collection (same).
 * The copy constructor may be overriden to provide more efficient copy for specific graphs.
 */
public class Graph<V,E> implements Iterable<V> {

	/* CONSTRUCTORS */

	public Graph(VertexCollection<V> vertices, EdgeCollection<E> edges) throws IllegalConstructionException {
		if ((vertices == null) || (edges == null))
			throw new IllegalConstructionException();
		_vertices = vertices;
		_edges = edges;
	}

	public Graph(Graph<V,E> g) throws IllegalConstructionException {
		Graph<V,E> gclone = g.clone();
		this._vertices = gclone._vertices;
		this._edges = gclone._edges;
	}

	public void clear() {
		_vertices.clear();
		_edges.clear();
	}
	
	public Graph<V,E> clone() {
		try {
			VertexCollection<V> vertices = _vertices.getClass().newInstance();
			EdgeCollection<E> edges = _edges.getClass().newInstance();
			Graph<V,E> graph = new Graph<V,E>(vertices,edges);
			for (V v : this)
				graph.addVertex(v);
			for (Iterator<Edge<E> > e = edgeIterator() ; e.hasNext() ; ) {
				Edge<E> edge = e.next();
				graph.addEdge(edge.getIDU(),edge.getIDV(),edge.getValue());
			}
			return graph;
		}
		catch (Exception e) {
			return null;
		}
	}			

	/**
	 * @return A subgraph composed by the vertices between idBegin (included) and idEnd (not included) and all the remaining edges 
	 */
	public Graph<V,E> subgraph(int idBegin, int idEnd) {
		try {
			if (idEnd > getNbVertices())
				idEnd = getNbVertices();
			if (idBegin < 0)
				idBegin = 0;
			return new Graph<V,E>(_vertices.subset(idBegin,idEnd),_edges.subset(idBegin,idEnd));
		}
		catch (IllegalConstructionException e) {
			return null;
		}
	}

	/* VERTICES */

	public int getNbVertices() {
		return _vertices.size();
	}

	public Vertex<V> getVertex(int id) throws NoSuchElementException {
		return _vertices.get(id);
	}

	public Vertex<V> getNeighbour(int vertexID, int index) throws UnsupportedOperationException {
		return getVertex(_edges.getNeighbourAt(vertexID,index));
	}

	public V getNeighbourValue(int vertexID, int index) throws UnsupportedOperationException {
		return getNeighbour(vertexID,index).getValue();
	}

	public void removeVertex(int id) {
		_edges.onVertexRemoved(id);
		_vertices.remove(id);
	}

	
	public V get(int id) throws NoSuchElementException {
		return getVertex(id).getValue();
	}

	public void add(V value) {
		addVertex(value);
	}

	public void addVertex(V value) {
		Vertex<V> v = new Vertex<V>(value);
		_vertices.add(v);					// this method should change vertex id to match its index
		_edges.onVertexAdded(v.getID());	// uses directly vertex class ?
	}

	public void contract(Vertex<V> u, Vertex<V> v) {
		contract(u.getID(),v.getID());
	}

	public void contract(int idU, int idV) {
		NeighbourEdge<E> edge = null;
		for (Iterator<NeighbourEdge<E> > iterator = neighbourIterator(idU) ; iterator.hasNext() ; ) {
			edge = iterator.next();
			try {addEdge(idV,edge.getIDV(),edge.getValue());}
			catch (IllegalEdgeException e) { }
		}
		setVertex(idU,getVertex(idV));
		_edges.onVertexContracted(idU,idV);
	}
	
	/** protected because no real check 
	 * (used by vertex contraction) */
	protected void setVertex(int i, Vertex<V> u) {
		_vertices.set(i,u);
	}

	/* EDGES */

	public int getNbEdges() {
		return _edges.size();
	}

	public boolean isEdge(int idU, int idV) {
		return _edges.contains(idU,idV);
	}

	public E getEdgeValue(int idU, int idV) throws NoSuchElementException {
		return _edges.getValue(idU,idV);
	}
	
	public E getEdgeValue(Vertex<V> u, Vertex<V> v) throws NoSuchElementException {
		return getEdgeValue(u.getID(),v.getID());
	}

	public Edge<E> getEdge(Vertex<V> u, Vertex<V> v) throws NoSuchElementException {
		return _edges.get(u.getID(),v.getID());
	}

	public Edge<E> getEdge(int idU, int idV) throws NoSuchElementException {
		return _edges.get(idU,idV);
	}

	public void addNeighbourEdge(int idU, NeighbourEdge<E> edge) throws NoSuchElementException, IllegalEdgeException {
		addEdge(idU, edge.getIDV(), edge.getValue());
	}

	public void addEdge(int idU, int idV, E value) throws NoSuchElementException, IllegalEdgeException {
		if ((idU >= getNbVertices()) || (idV >= getNbVertices()))
			throw new NoSuchElementException();
		_edges.add(idU, idV, value);
	}

	public void addEdge(Vertex<V> u, Vertex<V> v, E value) throws NoSuchElementException, IllegalEdgeException {
		addEdge(u.getID(),v.getID(),value);
	}

	public void addEdge(Edge<E> edge) throws NoSuchElementException, IllegalEdgeException {
		if ((edge.getIDU() >= getNbVertices()) || (edge.getIDV() >= getNbVertices()))
			throw new NoSuchElementException();
		_edges.add(edge);
	}

	public void removeEdge(int idU, int idV) throws NoSuchElementException {
		if ((idU >= getNbVertices()) || (idV >= getNbVertices()))
			throw new NoSuchElementException();
		_edges.remove(idU,idV);
	}

	public void removeEdge(Vertex<V> u, Vertex<V> v) throws NoSuchElementException {
		removeEdge(u.getID(),v.getID());
	}


	/** ITERATORS */

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
		return new DFSIterator(this,0,null);
	}

	public WalkIterator DFSIterator(int root) {
		return new DFSIterator(this,root,null);
	}

	public WalkIterator DFSIterator(int root, VertexBinaryFunction<V> function) {
		return new DFSIterator(this,root,function);
	}


	/* ALGORITHMS */

	public ParentFunction<V> Dijsktra(int root, E zeroValue, OperatorPlus1T<E> plus, Comparator<E> compareEdge) {
		ArrayList<Vertex<V> > ends = new ArrayList<Vertex<V> >(0);
		return AStar(root,
					 zeroValue,
					 plus,
					 compareEdge,
					 ends,
					 null);
	}

	public ParentFunction<V> AStar(int root, 
								   E zeroValue,
								   OperatorPlus1T<E> plus,
								   Comparator<E> compareEdge,
								   ArrayList<Vertex<V> > ends, 
								   ArrayList<E> heuristique) {
		ArrayList<E> weights = new ArrayList<E>(getNbVertices());	// contains the total weights of edges between root and index vertices
		for (int i = 0 ; i < getNbVertices() ; i++)
			weights.add(null);
		weights.set(root,zeroValue);
		Vertex<V> u = null;
		NeighbourEdge<E> e = null;
		Graph<V,E>.AStarVertexComparator compareVertex = new AStarVertexComparator(zeroValue,plus,compareEdge,weights,heuristique);
		PriorityQueue<Vertex<V> > queue = new PriorityQueue<Vertex<V> >(11,compareVertex);
		ParentFunction<V> parent = new ParentFunction<V>(getNbVertices());
		u = getVertex(root);
		while (u != null) {
			if (ends.contains(u))
				return parent;
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
	private class AStarVertexComparator implements Comparator<Vertex<V> > {
		public AStarVertexComparator(E zeroValue, OperatorPlus1T<E> plus, Comparator<E> compareEdge, ArrayList<E> weights, ArrayList<E> heuristique) {
			_zeroValue = zeroValue;
			_weights = weights;
			_heuristique = heuristique;
			_plus = plus;
			_compareEdge = compareEdge;
		}
		/* the specific check is here to avoid to find multiple paths.
		 * furthermore it will reduce the number of iterations needed to find it.
		 */
		public int compare(Vertex<V> u, Vertex<V> v) {
			int result = _compareEdge.compare(get(u.getID()),get(v.getID()));
			if (result == 0)	// specific check against same values
				result = _compareEdge.compare(heuristique(u.getID()),heuristique(v.getID()));
			return result;
		}
		public E get(int id) {
			return _plus.exec(_weights.get(id),heuristique(id));
		}
		public E heuristique(int id) {
			try {
				return _heuristique.get(id);
			}
			catch (Exception e) {
				return _zeroValue;
			}
		}
		private ArrayList<E> _weights;
		private ArrayList<E> _heuristique;
		private E _zeroValue;
		private OperatorPlus1T<E> _plus;
		private Comparator<E> _compareEdge;
	}


	protected VertexCollection<V> _vertices = null;
	protected EdgeCollection<E> _edges = null;


	/**
	 * Graph vertex value iterator nested class.
	 * It is used to provide a generic value iterator based on the vertex collection one.
	 * You have just to override the vertexIterator() graph method to make this one works.
	 */
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

	/**
	 * Graph walk iterator nested class.
	 * This class is a generic walk algorithm iterator, its waiting list been unimplemented, 
	 * this class cannot be instantiated, use BFSIterator or DFSIterator instead of.
	 */
	public abstract class WalkIterator implements Iterator<Vertex<V> > {

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
				_colors[rootID] = 1;
			}
		}

		protected WalkIterator(Graph<V,E> source, Vertex<V> root, VertexBinaryFunction<V> function) {
			if (source.getNbVertices() == 0)
				_current = null;
			else {
				_source = source;
				_current = root;
				_function = function;
				_colors = new int[source.getNbVertices()];
				_neighbourIterators = new ArrayList<Iterator<NeighbourEdge<E> > >(source.getNbVertices());
				for (int i = 0 ; i < source.getNbVertices() ; i++) {
					_neighbourIterators.add(_source.neighbourIterator(i));
					_colors[i] = 0;
				}
				_colors[root.getID()] = 1;
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

	/**
	 * Graph breadth first search iterator nested class.
	 * It extends the walk iterator, and provide a FIFO waiting list.
	 */
	protected class BFSIterator extends WalkIterator {
		public BFSIterator(Graph<V,E> source, int root, VertexBinaryFunction<V> function) {
			super(source,root,function);
			_waitingList = new Fifo<Vertex<V> >();
			_waitingList.put(_current);
		}
	};

	/**
	 * Graph depth first search iterator nested class.
	 * It extends the walk iterator, and provide a LIFO waiting list.
	 */
	protected class DFSIterator extends WalkIterator {
		public DFSIterator(Graph<V,E> source, int root, VertexBinaryFunction<V> function) {
			super(source,root,function);
			_waitingList = new Lifo<Vertex<V> >();
			_waitingList.put(_current);
		}
	};

};

