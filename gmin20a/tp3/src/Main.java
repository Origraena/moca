import tictactoe.*;

public class Main {

	public static void main(String args[]) {
		TicTacToe tictactoe = null; 
		if (args.length == 0) {
			tictactoe = new TicTacToe();
		}
		else if (args[0].equals("q1")) {
			System.out.println("Question 1");
			System.out.println("AI : minimax, profondeur maximum");
			System.out.println("Grille : taille 3");
			TicTacToeAI tictactoeai = new TicTacToeAI(TicTacToeGrid.P2,0,1);
			tictactoe = new TicTacToe(new TicTacToeHuman(),tictactoeai,3);
		}
		else if (args[0].equals("q2")) {
			System.out.println("Question 2");
			System.out.println("AI : minimax, profondeur 3");
			System.out.println("Heuristique : somme de la valeur de chaque ligne, colonne, et diagonale (si un seul joueur occupe => nombre de cases remplies, sinon 0)");
			System.out.println("Grille : taille 5");
			TicTacToeAI tictactoeai = new TicTacToeAI(TicTacToeGrid.P2,3,1);
			tictactoe = new TicTacToe(new TicTacToeHuman(),tictactoeai,5);
		}
		else if (args[0].equals("q3")) {
			System.out.println("Question 1");
			System.out.println("AI : minimax, profondeur maximum");
			System.out.println("Heuristique : somme de la valeur de chaque ligne, colonne, et diagonale (si un seul joueur occupe => nombre de cases remplies, sinon 0)");
			System.out.println("Grille : taille 3");
			TicTacToeAI tictactoeai = new TicTacToeAI(TicTacToeGrid.P2,3,2);
			tictactoe = new TicTacToe(new TicTacToeHuman(),tictactoeai,5);
		}
		try {
			tictactoe.run();
		}
		catch (Exception e) {
			System.out.println(e);
			e.printStackTrace();
		}
	}

};

