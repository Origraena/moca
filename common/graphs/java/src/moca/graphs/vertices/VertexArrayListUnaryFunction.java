package moca.graphs.vertices;

import java.lang.String;
import java.util.ArrayList;

public class VertexArrayListUnaryFunction<V> extends VertexIdentityFunction<V> {

	public VertexArrayListUnaryFunction(int nbVertices) {
		_stringValues = new ArrayList<String>(nbVertices);
		for (int i = 0 ; i < nbVertices ; i++)
			_stringValues.add("");
	}

	public void set(Vertex<V> vertex, String value) {
		_stringValues.set(vertex.getID(),value);
	}

	public String exec(Vertex<V> vertex) {
		return _stringValues.get(vertex.getID());
	}

	private ArrayList<String> _stringValues;

};

