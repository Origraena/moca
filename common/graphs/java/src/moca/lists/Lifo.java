package moca.lists;

public class Lifo<E> extends LinkedList<E> {
	
	private static final long serialVersionUID = 2011022601L;

	public E get() {
		return super.getLast();
	}

	public E pop() {
		return super.pollLast();
	}

};

