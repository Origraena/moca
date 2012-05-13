package moca.tree;

public abstract class NodeFunction {

	public static final int MIN_VALUE = -30000;
	public static final int MAX_VALUE = 30000;

	public final int exec(DynamicTree.Node n) {
		if (n.gisLeaf())
			return leafValue(n);
		return nodeValue(n);
	}

	protected abstract int leafValue(DynamicTree.Node n);
	protected abstract int nodeValue(DynamicTree.Node n);
	public int maxValue() {
		return MAX_VALUE;
	}
	public int minValue() {
		return MIN_VALUE;
	}

};

