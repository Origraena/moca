package moca.graphs.edges;

public class Edge<Value> {

	public Edge(int idU, int idV, Value value) {
		_idU = idU;
		_idV = idV;
		_value = value;
	}
	
	public int getIDU() {
		return _idU;
	}

	public int getIDV() {
		return _idV;
	}

	public Value getValue() {
		return _value;
	}
	
	public void setIDU(int value) {
		_idU = value;
	}
	
	public void setIDV(int value) {
		_idV = value;
	}

	public void setValue(Value value) {
		_value = value;
	}

	public String toString() {
		return new String(_idU + "," + _idV + ":" + _value);
	}

	private int _idU;
	private int _idV;	
	private Value _value;

};

