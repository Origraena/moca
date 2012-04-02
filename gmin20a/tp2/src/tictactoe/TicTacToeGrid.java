package tictactoe;

import moca.lists.Matrix2;

import java.lang.Integer;
import java.util.NoSuchElementException;

public class TicTacToeGrid {

	public static final Integer FREE = null;
	public static final Integer P1 = new Integer(1);
	public static final Integer P2 = new Integer(2);

	public TicTacToeGrid(int width) {
		_grid = new Matrix2<Integer>(width,width);
	}
	
	public int getWidth() {
		return _grid.getWidth();
	}

	public int getSize() {
		return _grid.size();
	}
	
	public Integer get(int index) throws NoSuchElementException {
		return _grid.get(index);
	}

	public Integer get(int column, int line) throws NoSuchElementException {
		return _grid.get(column,line);
	}

	public String toString() {
		StringBuilder string = new StringBuilder();
		Integer current = FREE;
		for (int i = 0 ; i < getWidth() ; i++) {
			for (int j = 0 ; j < getWidth() ; j++) {
				current = _grid.get(i,j);
				if (current == FREE)
					string.append(" ");
				else
					string.append(current);
				string.append(',');
			}
			string.append('\n');
		}
		return string.toString();
	}


	/* Computations */

	public void play(int index) throws IllegalMoveException {
		play(_grid.columnOf(index),_grid.lineOf(index));
	}

	public void play(int column, int line) throws IllegalMoveException {
		try {
			if (hasEnded())
				throw new IllegalMoveException();
			if (_grid.get(column,line) != FREE)
				throw new IllegalMoveException();
			_grid.set(column,line,_current);
			_current = nextPlayer();
		}
		catch (NoSuchElementException e) {
			throw new IllegalMoveException();
		}
	}

	public Integer nextPlayer() {
		if (_current == P1)
			return P2;
		if (_current == P2)
			return P1;
		return null;
	}

	public boolean isFull() {
		for (Integer tile : _grid)
			if (tile == FREE)
				return false;
		return true;
	}

	public boolean hasEnded() {
		return ((winner() != null) || (isFull()));
	}

	public Integer winner() {
		Integer winner = firstDiagonalWinner();
		if (winner == null)
			winner = secondDiagonalWinner();
		for (int i = 0 ; (i < getWidth()) && (winner == null) ; i++) {
			winner = lineWinner(i);
			if (winner == null)
				winner = columnWinner(i);
		}
		return winner;
	}

	public Integer lineWinner(int line) {
		Integer winner = _grid.get(0,line);
		for (int i = 1 ; (i < getWidth()) && (winner != null) ; i++) {
			if (get(i,line) != winner)
				winner = null;
		}
		return winner;
	}

	public Integer columnWinner(int column) {
		Integer winner = _grid.get(column,0);
		for (int i = 1 ; (i < getWidth()) && (winner != null) ; i++) {
			if (get(column,i) != winner)
				winner = null;
		}
		return winner;
	}

	public Integer firstDiagonalWinner() {
		Integer winner = _grid.get(0,0);
		for (int i = 1 ; (i < getWidth()) && (winner != null) ; i++) {
			if (get(i,i) != winner)
				winner = null;
		}
		return winner;
	}

	public Integer secondDiagonalWinner() {
		int width = getWidth();
		Integer winner = _grid.get(0,width-1);
		for (int i = 1 ; (i < width) && (winner != null) ; i++) {
			if (get(i,width-i-1) != winner)
				winner = null;
		}
		return winner;
	}

	public TicTacToeGrid clone() {
		TicTacToeGrid clone = new TicTacToeGrid(getWidth());
		clone._current = _current;
		int i = 0;
		for (Integer tile : _grid) {
			clone.set(i,tile);
			i++;
		}
		return clone;
	}

	public boolean equals(Object o) {
		if (!(o instanceof TicTacToeGrid))
			return false;
		TicTacToeGrid grid = (TicTacToeGrid) o;
		if (grid.getWidth() != getWidth())
			return false;
		for (int i = 0 ; i < getSize() ; i++)
			if (get(i) != grid.get(i))
				return false;
		return true;
	}

	private void set(int index, Integer value) {
		try {
			_grid.set(index,value);
		}
		catch (NoSuchElementException e) { }
	}

	private Matrix2<Integer> _grid;
	private Integer _current = P1;

};

