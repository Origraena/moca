package moca.graphs.vertices;

import java.lang.String;

public class Vertex<Value> {

	
	public static final int ID_UNDEFINED = -1;

	public Vertex(int id, Value value) {
		_id = id;
		_value = value;
	}

	public Vertex(Value value) {
		_value = value;
	}

	public int getID() {
		return _id;
	}

	public void setID(int value) {
		_id = value;
	}

	public Value getValue() {
		return _value;
	}

	public void setValue(Value value) {
		_value = value;
	}

	public String toString() {
		return new String(_id + ":" + _value);
	}

	public Vertex<Value> clone() {
		return new Vertex<Value>(_id,_value);
	}

	private int _id = ID_UNDEFINED;

	private Value _value;

};

