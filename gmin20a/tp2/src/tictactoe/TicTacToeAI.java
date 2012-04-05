package tictactoe;

public class TicTacToeAI implements TicTacToePlayer {

	public TicTacToeAI(Integer player) {
		_tree = new TicTacToeTree(player);
		_function = new TicTacToeFunction(player);
	}

	public TicTacToeAI(Integer player, int maxDepth) {
		_tree = new TicTacToeTree(player);
		_function = new TicTacToeFunction(player);
		_maxDepth = maxDepth;
	}

	public TicTacToeAI(Integer player, int maxDepth, boolean minimax) {
		_tree = new TicTacToeTree(player);
		_function = new TicTacToeFunction(player);
		_maxDepth = maxDepth;
		_minimax = minimax;
	}


	public TicTacToeAI(Integer player, int maxDepth, TicTacToeFunction function) {
		_tree = new TicTacToeTree(player);
		_function = function;
		_maxDepth = maxDepth;
		_function.setPlayer(player);
	}

	public void init() {
		_currentNode = null;
	}

	public TicTacToeGrid play(TicTacToeGrid grid) {
		if (_currentNode == null)
			_currentNode = _tree.node(grid);
		else {
			for (TicTacToeTree.Node n : _tree.children(_currentNode)) {
				if (grid.equals(n.getValue())) {
					_currentNode = n;
					break;
				}
			}
		}
		_currentNode = nextNode();
		return currentGrid();
	}

	protected TicTacToeTree.Node nextNode() {
		if (_minimax)
			return _tree.minimax(_currentNode,_maxDepth,_function);
		return _tree.negamax(_currentNode,_maxDepth,_function);
	}

	protected TicTacToeGrid currentGrid() {
		if (_currentNode == null)
			return null;
		return (TicTacToeGrid) _currentNode.getValue();
	}

	private TicTacToeTree _tree;
	private TicTacToeTree.Node _currentNode;
	private TicTacToeFunction _function;
	private int _maxDepth = 0;
	private boolean _minimax = true;

};
