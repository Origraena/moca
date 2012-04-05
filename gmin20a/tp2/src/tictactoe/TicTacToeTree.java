package tictactoe;

import moca.tree.DynamicTree;

import java.util.ArrayList;

public class TicTacToeTree extends DynamicTree<TicTacToeGrid> {

	public TicTacToeTree(Integer player) {
		_player = player;
	}

	public ArrayList<Node> children(Node n) {
		return (ArrayList<Node>)super.children(n);
	}

	public ArrayList<Node> processChildren(Node n) {
		TicTacToeGrid grid = n.getValue();
		TicTacToeGrid child = null;
		ArrayList<Node> result = new ArrayList<Node>();
		if (grid.winner() == null) {
			for (int i = 0 ; i < grid.getSize() ; i++) {
				try {
					child = grid.clone();
					child.play(i);
					result.add(new Node(child));
				}
				catch (IllegalMoveException e) {
					// nothing to be done
				}
			}
		}
		return result;
	}

	public Node parent(Node n) throws UnsupportedOperationException {
		throw new UnsupportedOperationException();
	}

	public boolean isLeaf(Node n) {
		return (children(n).size() == 0);
	}

	public boolean isMinNode(Node n) {
		return n.getValue().nextPlayer() != _player;
	}

	public Node node(TicTacToeGrid value) {
		return new Node(value);
	}

	private Integer _player = null;

};

