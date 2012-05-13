package moca.lists;

public class Fifo<E> extends LinkedList<E> {

	private static final long serialVersionUID = 2011022602L;

	public E get() {
		return super.getFirst();
	}

	public E pop() {
		return super.pollFirst();
	}

};

