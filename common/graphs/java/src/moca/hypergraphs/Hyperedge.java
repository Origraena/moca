package moca.hypergraphs;

public class HyperEdge<E> {

	public HyperEdge(int size) {
		if (size > 0) {
			_size = size;
			_verticesID = new int[size];
		}
	}
	
	public HyperEdge(int size, E value) {
		if (size > 0) {
			_size = size;
			_verticesID = new int[size];
		}
		_value = value;
	}
	
	public HyperEdge(ArrayList<Integer> verticesID, E value) {
		if ((verticesID != null) && (verticesID.size() > 0)) {
			_size = verticesID.size();
			_verticesID = new int[_size];
			for (int i = 0 ; i < _size ; i++)
				_verticesID[i] = verticesID.get(i).intValue();
		}
		_value = value;
	}

	public E getValue() {
		return _value;
	}
	
	public E setValue(E value) {
		E old = _value;
		if (_value != value)
			_value = value;
		return old;
	}
	
	public int getSize() {
		return _size;
	}
	
	public void setSize(int size) {
		if (_size != size) {
			if (_size > size) {
				int[] tmp = new int[size];
				for (int i = 0 ; i < size ; i++)
					tmp[i] = _verticesID[i];
				_verticesID = tmp;
			}
			else {
				int[] tmp = new int[size];
				for (int i = 0 ; i < _size ; i++)
					tmp[i] = verticesID[i];
				for (int i = _size ; i < size ; i++)
					tmp[i] = -1;
				_verticesID = tmp;
			}
			_size = size;
		}
	}
	
	public int getVertexID(int index) throws NoSuchElementException {
		if ((index < 0) || (index >= _size) || (_verticesID[index] < 0))
			throw new NoSuchElementException();
		return _verticesID[index];
	}

	public int setVertexID(int index, int id) throws NoSuchElementException {
		if ((index < 0) || (index >= _size))
			throw new NoSuchElementException();
		int old = _verticesID[index];
		_verticesID[index] = id;
		return old;
	}

	private int _size = 0;
	private int[] _verticesID = null;
	private E _value = null;

};

