package moca.graphs.visu;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexIdentityFunction;

public class SCCIdentityFunction extends VertexIdentityFunction {
	public String exec(Vertex v) {
		return "C"+v.getID();
	}
	public static SCCIdentityFunction instance() {
		if (_instance == null)
			_instance = new SCCIdentityFunction();
		return _instance;
	}
	private static SCCIdentityFunction _instance = null;
	protected SCCIdentityFunction() { }
};

