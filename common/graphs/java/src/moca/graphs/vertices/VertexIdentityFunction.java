package moca.graphs.vertices;

import java.lang.String;

public class VertexIdentityFunction<V> implements VertexUnaryFunction<String,V> {

	public String exec(Vertex<V> v) {
		return String.valueOf(v.getID());
	}

};

