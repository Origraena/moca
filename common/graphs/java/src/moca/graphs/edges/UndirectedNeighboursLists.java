package moca.graphs.edges;

import java.util.NoSuchElementException;


public class UndirectedNeighboursLists<E> extends NeighboursLists<E> implements EdgeCollection<E> {

	public E getValue(int idU, int idV) throws NoSuchElementException {
		try {
			return super.getValue(idU,idV);
		}
		catch (NoSuchElementException exception) {
			return super.getValue(idV,idU);
		}
	}

	public void add(int idU, int idV, E value) throws NoSuchElementException {
		if ((idU >= _neighbours.size()) || (idV >= _neighbours.size()))
			throw new NoSuchElementException();
		_neighbours.get(idU).add(new NeighbourEdge<E>(idV,value));
		_neighbours.get(idV).add(new NeighbourEdge<E>(idU,value));
		_size++;
	}

	public void removeOne(int idU, int idV) throws NoSuchElementException {
		super.remove(idU,idV);
	}


	public void remove(int idU, int idV) throws NoSuchElementException {
		int cpt = internalRemove(idU,idV);
		if (cpt == 0)
			throw new NoSuchElementException();
		_size -= internalRemove(idV,idU);
	}

	protected int internalRemove(int idU, int idV) {
		int cpt = 0;
		for (int i = 0 ; i < _neighbours.get(idU).size() ; i++) {
			if (_neighbours.get(idU).get(i).getIDV() == idV) {
				_neighbours.get(idU).remove(i);
				i--;
				cpt++;
			}
		}
		return cpt;
	}


};

