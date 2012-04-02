package moca.lists;

import java.lang.Iterable;
import java.util.NoSuchElementException;

public class Matrix2<E> implements Iterable<E> {

	public Matrix2(int height, int width) {
		_height = height;
		_width = width;
		_matrix = new Object[height][];
		for (int i = 0 ; i < height ; i++) {
			_matrix[i] = new Object[width];
			for (int j = 0 ; j < width ; j++)
				_matrix[i][j] = null;
		}
	}
	
	public int getHeight() {
		return _height;
	}

	public int getWidth() {
		return _width;
	}

	public int size() {
		return _height*_width;
	}

	public E get(int index) throws NoSuchElementException {
		return get(columnOf(index),lineOf(index));
	}

	public E get(int column, int line) throws NoSuchElementException {
		if ((column < 0) || (line < 0) || (column >= _width) || (line >= _height))
			throw new NoSuchElementException();
		return (E) _matrix[line][column];
	}

	public E set(int index, E value) throws NoSuchElementException {
		return set(columnOf(index),lineOf(index),value);
	}

	public E set(int column, int line, E value) throws NoSuchElementException {
		E oldValue = get(column,line);
		_matrix[line][column] = value;
		return oldValue;
	}

	public E remove(int index) throws NoSuchElementException {
		return set(index,null);
	}

	public E remove(int column,int line) throws NoSuchElementException {
		return set(column,line,null);
	}

	public Iterator iterator() {
		return new Iterator();
	}


	/* Index */
	public int globalIndex(int column, int line) {
		return line*_width + column;
	}
	public int lineOf(int index) {
		return index / _width;
	}
	public int columnOf(int index) {
		int result = index % _width;
		if (result < 0)
			result += _width;
		return result;
	}

	private Object _matrix[][] = null;
	private int _height = 0;
	private int _width = 0;


	public class Iterator implements java.util.Iterator<E> {
		public boolean hasNext() {
			return _current < size();
		}
		public E next() throws NoSuchElementException {
			if (!hasNext())
				throw new NoSuchElementException();
			_current++;
			return get(_current-1);
		}
		public void remove() throws NoSuchElementException {
			//remove(_current);		// TODO pointer on encapsulating class
		}
		private int _current = 0;
	};

};

