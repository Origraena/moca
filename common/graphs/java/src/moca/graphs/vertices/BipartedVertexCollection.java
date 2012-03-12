package moca.graphs.vertices;

import moca.graphs.IllegalConstructionException;

import java.util.Collection;
import java.util.NoSuchElementException;

/**
 * Represents a biparted vertex set for a graph.
 * It takes two VertexCollection at construction.
 * The addInFirstSet operation is less effective if the second set have lots of elements.
 * That is why the first set should be the biggest of the both sets.
 * Or, another way, if the collection does not make heavy changes, the first set will have to be generated before the second one.
 */
public class BipartedVertexCollection<Value> implements VertexCollection<Value> {

	public BipartedVertexCollection(VertexCollection<Value> firstSet, VertexCollection<Value> secondSet) throws IllegalConstructionException {
		if ((firstSet == null) || (secondSet == null))
			throw new IllegalConstructionException();
		_firstSet = firstSet;
		_secondSet = secondSet;
	}

	public int size() {
		return _firstSet.size() + _secondSet.size();
	}

	public BipartedVertexCollection<Value> subset(int idBegin, int idEnd) {
		if (idBegin > _firstSet.size())
			return subset(0,0,idBegin-_firstSet.size(),idEnd-_firstSet.size());
		if (idEnd <= _firstSet.size())
			return subset(idBegin,idEnd,0,0);
		return subset(idBegin,_firstSet.size(),0,idEnd-size());
	}

	public BipartedVertexCollection<Value> subset(int idBegin1, int idEnd1, int idBegin2, int idEnd2) {
		try {
			return new BipartedVertexCollection<Value>(_firstSet.subset(idBegin1,idEnd1),_secondSet.subset(idBegin2,idEnd2));
		}
		catch (IllegalConstructionException e) {
			return null;
		}
	}

	/**
	 * this method should not be used since it is not really specific
	 * the vertex will be added into the second set
	 */
	public boolean add(Vertex<Value> v) {
		return _secondSet.add(v);	
	}

	public void addInFirstSet(Vertex<Value> v) {
		_firstSet.add(v);
		v.setID(_firstSet.size()-1);
		int i = _firstSet.size();
		for (Vertex<Value> u : _secondSet) {
			u.setID(i);
			i++;
		}
	}

	public void addInSecondSet(Vertex<Value> v) {
		_secondSet.add(v);
		v.setID(size()-1);
	}

	/** global id **/
	public Vertex<Value> remove(int id) throws NoSuchElementException {
		if (id < _firstSet.size()) 
			return removeInFirstSet(id);
		else if (id < size())
			return removeInSecondSet(id - _firstSet.size());
		else
			throw new NoSuchElementException();
	}

	/** local id ***/
	public Vertex<Value> removeInFirstSet(int id) throws NoSuchElementException {
		Vertex<Value> v = _firstSet.remove(id);
		int i = _firstSet.size();
		for (Vertex<Value> u : _secondSet) {
			u.setID(i);
			i++;
		}
		return v;
	}


	/** local id **/
	public Vertex<Value> removeInSecondSet(int id) throws NoSuchElementException {
		return _secondSet.remove(id);
	}

	public Vertex<Value> set(int id, Vertex<Value> v) {
		try {
			Vertex<Value> result = get(id);
			if (id < _firstSet.size())
				_firstSet.set(id,v);
			else if (id < size())
				_secondSet.set(id - _firstSet.size(),v);
			return result;
		}
		catch (Exception e) {
			return null;
		}
	}

	public Vertex<Value> get(int id) throws NoSuchElementException {
		if (id < _firstSet.size())
			return _firstSet.get(id);
		else if (id < size())
			return _secondSet.get(id - _firstSet.size());
		else
			throw new NoSuchElementException();
	}

	/** local id **/
	public Vertex<Value> getInFirstSet(int id) throws NoSuchElementException {
		if (id >= _firstSet.size())
			throw new NoSuchElementException();
		return _firstSet.get(id);
	}

	/**
	 * local id
	 */
	public Vertex<Value> getInSecondSet(int id) {
		if (id >= _secondSet.size())
			throw new NoSuchElementException();
		return _secondSet.get(id);
	}

	public VertexCollection<Value> firstSet() {
		return _firstSet;
	}

	public VertexCollection<Value> secondSet() {
		return _secondSet;
	}

	public java.util.Iterator<Vertex<Value> > iterator() {
		return new Iterator(_firstSet.iterator(), _secondSet.iterator());
	}

	public void clear() {
		_firstSet.clear();
		_secondSet.clear();
	}

	private VertexCollection<Value> _firstSet = null;
	private VertexCollection<Value> _secondSet = null;

	
	protected class Iterator implements java.util.Iterator<Vertex<Value> > {

		public Iterator(java.util.Iterator<Vertex<Value> > firstIterator, java.util.Iterator<Vertex<Value> > secondIterator) {
			_firstIterator = firstIterator;
			_secondIterator = secondIterator;
		}

		public Vertex<Value> next() throws NoSuchElementException { 
			if (_firstIterator.hasNext())
				return _firstIterator.next();
			else if (_secondIterator.hasNext())
				return _secondIterator.next();
			else
				throw new NoSuchElementException();
		}

		public boolean hasNext() {
			return (_firstIterator.hasNext() || _secondIterator.hasNext());
		}

		// TODO a verifier
		// remove() supprime le dernier appel a next ou le prochain ?
		public void remove() throws NoSuchElementException, UnsupportedOperationException {
			if (_firstIterator.hasNext())
				_firstIterator.remove();
			else if (_secondIterator.hasNext())
				_secondIterator.remove();
			else
				throw new NoSuchElementException();
		}

		private java.util.Iterator<Vertex<Value> > _firstIterator = null;
		private java.util.Iterator<Vertex<Value> > _secondIterator = null;

	};

};

