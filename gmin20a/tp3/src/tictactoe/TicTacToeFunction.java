package tictactoe;

import moca.tree.DynamicTree;
import moca.tree.NodeFunction;

public class TicTacToeFunction extends NodeFunction {

	public static final Integer FREE = TicTacToeGrid.FREE;

	public TicTacToeFunction(Integer player) {
		super();
		_player = player;
	}

//	public boolean isCurrentPlayer(Integer player, TicTacToeGrid grid) {
//		return (player == grid.nextPlayer());
//	}

	public boolean isPlayer(Integer player) {
		return _player == player;
	}

	public void setPlayer(Integer player) {
		_player = player;
	}

	protected int leafValue(DynamicTree.Node n) {
		TicTacToeGrid grid = ((TicTacToeTree.Node)n).getValue();
		Integer winner = grid.winner();
		if (winner == null)
			return 0;				
		if (isPlayer(winner))
			return grid.getSize()*grid.getSize()*grid.getSize(); 
		return 0 - (grid.getSize()*grid.getSize()*grid.getSize());
	}

	protected int nodeValue(DynamicTree.Node n) {
		TicTacToeGrid grid = ((TicTacToeTree.Node)n).getValue();
		int value = firstDiagonalValue(grid);
		value += secondDiagonalValue(grid);
		for (int i = 0 ; i < grid.getWidth() ; i++) {
			value += lineValue(grid,i);
			value += columnValue(grid,i);
		}
		return value;
	}

	protected int lineValue(TicTacToeGrid grid, int line) {
		int width = grid.getWidth();
		Integer player = FREE;
		int value = 0;
		Integer current = FREE;
		for (int i = 0 ; i < width ; i++) {
			current = grid.get(line,i);
			if (current != FREE) {
				if (player == FREE)
					player = current;
				if (current == player)
					value++;
				else
					return 0;
			}
		}
		if (isPlayer(player))
			return value;
		else
			return 0 - value;
	}

	protected int columnValue(TicTacToeGrid grid, int column) {
		int width = grid.getWidth();
		Integer player = FREE;
		int value = 0;
		Integer current = FREE;
		for (int i = 0 ; i < width ; i++) {
			current = grid.get(column,i);
			if (current != FREE) {
				if (player == FREE)
					player = current;
				if (current == player)
					value++;
				else
					return 0;
			}
		}
		if (isPlayer(player))
			return value;
		else
			return 0 - value;
	}

	protected int firstDiagonalValue(TicTacToeGrid grid) {
		int width = grid.getWidth();
		Integer player = FREE;
		int value = 0;
		Integer current = FREE;
		for (int i = 0 ; i < width ; i++) {
			current = grid.get(i,i);
			if (current != FREE) {
				if (player == FREE)
					player = current;
				if (current == player)
					value++;
				else
					return 0;
			}
		}
		if (isPlayer(player))
			return value;
		else
			return 0 - value;
}

	protected int secondDiagonalValue(TicTacToeGrid grid) {
		int width = grid.getWidth();
		int value = 0;
		Integer current = FREE;
		Integer player = FREE;
		for (int i = 0 ; i < width ; i++) {
			current = grid.get(i,width-i-1);
			if (current != FREE) {
				if (player == FREE)
					player = current;
				if (current == player)
					value++;
				else
					return 0;
			}
		}
		if (isPlayer(player))
			return value;
		else
			return 0 - value;
	}

	private Integer _player;

};

