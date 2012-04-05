package tictactoe;

import java.util.Scanner;

public class TicTacToeHuman implements TicTacToePlayer {

	public void init() { }

	public TicTacToeGrid play(TicTacToeGrid grid) {
		int tile;
		System.out.println("Choose a tile :");
		try {
			tile = _scan.nextInt();
		}
		catch (Exception e) {
			System.out.println("Input error !");
			return play(grid);
		}
		try {
			grid.play(tile-1);
			return grid;
		}
		catch (IllegalMoveException e) {
			System.out.println("Choose a REAL UNUSED tile... :");
			return play(grid);
		}
	}

	private Scanner _scan = new Scanner(System.in);

};

