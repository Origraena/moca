package moca.graphs.edges;

public class NeighbourEdge<E> {
	
		public NeighbourEdge(int id, E val) {
			_idV = id;
			_value = val;
		}

		public int getIDV() {
			return _idV;
		}

		public E getValue() {
			return _value;
		}

		public void setIDV(int value) {
			_idV = value;
		}

		public void setValue(E value) {
			_value = value;
		}

		private int _idV;

		private E _value;
	
};

