package moca.graphs.vertices;

import moca.graphs.Point;
import moca.graphs.edges.NeighbourEdge;

import java.util.ArrayList;
import java.util.NoSuchElementException;

/**
 * This collection has a fixed size.
 */
public class VertexGrid<V,E> implements VertexEdgedCollection<V,E> {

	public static final int NONE = -1;
	public static final int LEFT = 0;
	public static final int TOP = 1;
	public static final int RIGHT = 2;
	public static final int BOTTOM = 3;


	public VertexGrid(int height, int width) {
		_grid = new ArrayList<ArrayList<Vertex<V> > >(height);
		_height = height;
		_width = width;
		_edges = new ArrayList<E>(size());
		Vertex<V> v;
		for (int i = 0 ; i < height ; i++) {
			_grid.add(new ArrayList<Vertex<V> >(width));
			for (int j = 0 ; j < width ; j++) {
				_edges.add(null);
				v = new Vertex<V>(null);
				v.setID(i*width+j);
				_grid.get(i).add(v);
			}
		}
	}

	public VertexGrid(int height, int width, E initEdgeValue) {
		_grid = new ArrayList<ArrayList<Vertex<V> > >(height);
		_height = height;
		_width = width;
		_edges = new ArrayList<E>(size());
		Vertex<V> v;
		for (int i = 0 ; i < height ; i++) {
			_grid.add(new ArrayList<Vertex<V> >(width));
			for (int j = 0 ; j < width ; j++) {
				_edges.add(initEdgeValue);
				v = new Vertex<V>(null);
				v.setID(i*width+j);
				_grid.get(i).add(v);
			}
		}
	}



	public VertexArrayList<V> subset(int idBegin, int idEnd) {
		VertexArrayList<V> result = new VertexArrayList<V>();
		for (int i = idBegin ; i < idEnd ; i++)
			result.add(get(i).clone());
		return result;
	}

	public int size() {
		return _width * _height;
	}

	public int getWidth() {
		return _width;
	}

	public int getHeight() {
		return _height;
	}

	public int getNbEdges() {
		int m = 0;
		for (int i = 0 ; i < size() ; i++) {
			for (int j = 0 ; j < 4 ; j++) {
				try {
					getNeighbourAt(i,j);
					m++;
				}
				catch (NoSuchElementException e) { }
			}
		}
		return m;
	}

	public void clear() { }

	public Vertex<V> get(int id) {
		Point p = localID(id);
		if (p == null)
			return null;
		return _grid.get(p.y).get(p.x);
	}

	public boolean add(Vertex<V> v) {
		return false;
	}

	public Vertex<V> remove(int id) throws UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}

	public Vertex<V> set(int id, Vertex<V> v) {
		Point p = localID(id);
		if (p != null)
			return _grid.get(p.y).set(p.x,v);
		return null;
	}

	public int globalID(int x, int y) {
		return (y * _width + x);
	}

	public Point localID(int id) {
		int y = id / _width;
		int x = id % _width;
		if (x < 0)
			x += _width;
		return new Point(x,y);
	}

	public int side(int idU, int idV) {
		Point pu = localID(idU);
		Point pv = localID(idV);
		if (pu.x == pv.x) {
			if (pu.y == (pv.y + 1))
				return BOTTOM;
			if (pu.y == (pv.y -1))
				return TOP;
			return NONE;
		}
		if (pu.y == pv.y) {
			if (pu.x == (pv.x + 1))
				return RIGHT;
			if (pu.x == (pv.x - 1))
				return LEFT;
		}
		return NONE;
	}
	
	public E getEdgeValue(int idU, int idV) throws NoSuchElementException {
		if (side(idU,idV) == NONE)
			throw new NoSuchElementException();
		if (_edges.get(idV) == null)
			throw new NoSuchElementException();
		return _edges.get(idV);
	}

	public void setEdgeValue(int idV, E value) {
		_edges.set(idV,value);
	}

	public void removeEdge(int idU, int idV) {
		setEdgeValue(idV,null);
	}

	public int getNbNeighbours(int id) throws NoSuchElementException {
		int cpt = 0;
		for (int i = 0 ; i < 4 ; i++) {
			try {
				getEdgeValue(id,getNeighbourAt(id,i));
				cpt++;
			}
			catch (NoSuchElementException e) { }
		}
		return cpt;
	}

	public int getNeighbourAt(int idU, int index) throws NoSuchElementException {
		int v = -1;
		switch (index) {
			case LEFT :
				v = getNeighbourAtLeft(idU);
				break;
			case TOP :
				v = getNeighbourAtTop(idU);
				break;
			case RIGHT :
				v = getNeighbourAtRight(idU);
				break;
			case BOTTOM :
				v = getNeighbourAtBottom(idU);
				break;
			default :
				throw new NoSuchElementException();
		}
		getEdgeValue(idU,v);			// will throw a NoSuchElementException if there is no edge
		return v;
	}

	public int getNeighbourAtLeft(int idU) throws NoSuchElementException {
		Point p = localID(idU);
		if (p.x < 0)
			throw new NoSuchElementException();
		return globalID(p.x-1,p.y);
	}

	public int getNeighbourAtTop(int idU) throws NoSuchElementException {
		Point p = localID(idU);
		if (p.y < 0)
			throw new NoSuchElementException();
		return globalID(p.x,p.y-1);
	}

	public int getNeighbourAtRight(int idU) throws NoSuchElementException {
		Point p = localID(idU);
		if (p.x >= _width-1)
			throw new NoSuchElementException();
		return globalID(p.x+1,p.y);
	}

	public int getNeighbourAtBottom(int idU) throws NoSuchElementException {
		Point p = localID(idU);
		if (p.y >= _height-1)
			throw new NoSuchElementException();
		return globalID(p.x,p.y+1);
	}

	public Iterator iterator() {
		return new Iterator();
	}

	public NeighbourIterator neighbourIterator(int id) {
		return new NeighbourIterator(id);
	}

	private ArrayList<ArrayList<Vertex<V> > > _grid;
	private final int _width;
	private final int _height;

	private ArrayList<E> _edges;


	public class Iterator implements java.util.Iterator<Vertex<V> > {
		public boolean hasNext() {
			return _current < size();
		}
		public Vertex<V> next() throws NoSuchElementException {
			if (!hasNext())
				throw new NoSuchElementException();
			_current++;
			return get(_current-1);
		}
		public void remove() throws UnsupportedOperationException {
			throw new UnsupportedOperationException();
		}
		private int _current = 0;
	};

	public class NeighbourIterator implements java.util.Iterator<NeighbourEdge<E> > {
		public NeighbourIterator(int id) {
			_id = id;
		}
		public boolean hasNext() {
			if (_index >= 4)
				return false;
			try {
				getNeighbourAt(_id,_index);
				return true;
			}
			catch (NoSuchElementException e) {
				return false;
			}
		}
		public NeighbourEdge<E> next() throws UnsupportedOperationException {
			if (!hasNext())
				throw new UnsupportedOperationException();
			int idV = getNeighbourAt(_id,_index);
			_index++;
			return new NeighbourEdge<E>(idV,getEdgeValue(_id,idV));
		}
		public void remove() throws UnsupportedOperationException {
			throw new UnsupportedOperationException();
		}
		private int _index = 0;
		private int _id;
	};

};

