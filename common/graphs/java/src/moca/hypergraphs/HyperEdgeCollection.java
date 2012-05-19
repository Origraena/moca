package moca.hypergraphs;

public interface HyperEdgeCollection<E> extends Iterable<E> {

	void onVertexAdded(int idV);

	void onVertexRemoved(int idV);

	void onVertexContracted(int idU, int idV);

	EdgeCollection<Value> subset(int idBegin, int idEnd);

	Edge<Value> get(int id) throws NoSuchElementException, UnsupportedOperationException;

	void remove (int id) throws NoSuchElementException, UnsupportedOperationException;

	int size();

	void clear();

	void add(HyperEdge<Value> edge) throws NoSuchElementException, IllegalEdgeException;

	boolean contains(HyperEdge<Value> edge);

	boolean remove(HyperEdge<Value> edge);

	Iterator<HyperEdge<Value> > iterator() throws UnsupportedOperationException;

};

