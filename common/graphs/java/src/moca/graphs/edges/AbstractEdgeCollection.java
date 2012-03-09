package moca.graphs.edges;

import java.lang.UnsupportedOperationException;
import java.util.NoSuchElementException;
import java.util.Collection;

public abstract class AbstractEdgeCollection<E> implements EdgeCollection<E> {

	public Edge<E> get(int idU, int idV) throws NoSuchElementException {
		return new Edge<E>(idU,idV,getValue(idU,idV));
	}

	public Edge<E> get(int id) throws UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}

	public void remove(int id) throws UnsupportedOperationException {
		throw new UnsupportedOperationException();	
	}


	/**
	 * Only checks vertex ids, not edge value.
	 */
	public boolean contains(Edge<E> edge) {
		try {
			getValue(edge.getIDU(),edge.getIDV());
			return true;
		}
		catch (Exception exception) {
			return false;
		}
	}

	public boolean contains(int idU, int idV) {
		try {
			getValue(idU, idV);
			return true;
		}
		catch (Exception exception) {
			return false;
		}
	}

	/**
	 * Only checks vertex ids, not edge value.
	 */
	public boolean remove(Edge<E> edge) {
		try {
			remove(edge.getIDU(), edge.getIDV());
			return true;
		}
		catch (NoSuchElementException exception) {
			return false;
		}
	}

	public void add(Edge<E> edge) throws NoSuchElementException, IllegalEdgeException {
		add(edge.getIDU(),edge.getIDV(),edge.getValue());
	}

};

