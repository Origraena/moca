package moca.lists;

public abstract class LinkedList<E> extends java.util.LinkedList<E> {

	public abstract E get();

	public abstract E pop();
	
	public void put(E e) {
		addLast(e);
	}

}


