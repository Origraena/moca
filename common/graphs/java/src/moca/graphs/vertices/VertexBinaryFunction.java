package moca.graphs.vertices;

public interface VertexBinaryFunction<V> {

	public void exec(Vertex<V> current, Vertex<V> neighbour);

};


