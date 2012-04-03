package tictactoe;

import java.lang.String;
import java.util.Scanner;

public class TicTacToeSUI implements TicTacToe.UI {

	/* Messages */

	public static final String MENU = "Menu :\n1\tNew Game\n2\tGame Options\n3\tUI Options\n4\tExit";
	public static final String NULL_MATCH = "Math null !";
	public static final String GAME_OPTIONS = "Game Options :";
	public static final String UI_OPTIONS = "UI Options :";
	public static final String NEW_GAME = "Game begins !";

	public static final String ASK_WIDTH = "Choose grid width -must be greater than or equals 3- :";
	public static final String ASK_NB_PLAYERS = "How many human will play ?";
	public static final String ASK_WHO_START = "Do you want to start ?";
	public static final String ASK_DIFFICULTY = "Choose AI difficulty -max depth of minimax tree (0 => leaves)- :";
	public static final String ASK_ALGORITHM = "Will use minimax ? -false implies negamax-";
	public static final String ASK_TILE_SIZE = "Choose a tile size : ";
	public static final String ASK_TOKEN = "Choose token :";
	public static final String ASK_VERTICAL_SEPARATOR = "Choose vertical separator :";
	public static final String ASK_HORIZONTAL_SEPARATOR = "Choose horizontal separator :";
	public static final String ASK_INTERSECTION = "Choose intersection :";

	public static final String AI_CREATION = "Select new AI player options :";
	public static final String HUMAN_CREATION = "Select new human player options :";

	public static final String ERROR = "/!\\ ERROR /!\\";
	public static final String WIDTH_ERROR = "The width must be greater than or equals 3 !";
	public static final String NB_PLAYERS_ERROR = "The number of players must be between 0 and 2 included !";

	public TicTacToeSUI(TicTacToe source) {
		_source = source;
	}

	public TicTacToeSUI(char p1, char p2, char verticalSeparator) {
		_token1 = p1;
		_token2 = p2;
		_verticalSeparator = verticalSeparator;
	}

	private TicTacToe _source;
	

	/* DISPLAY */

	public void displayMenu() {
		System.out.println(MENU);
	}
	public void displayGrid() {
		System.out.println(gridToString(_source.getGrid()));
	}
	public void displayWinner() {
		System.out.println(endToString(_source.getGrid().winner()));
	}
	public void displayError(String error) {
		System.out.println(ERROR+'\n'+error);
	}

	public String endToString(Integer winner) {
		String winnerStr = null;
		if (winner == null)
			return NULL_MATCH; 
		else
			winnerStr = winner.toString();
		return 'P' + winnerStr + " has won !\n";
	}
	public String gridToString(TicTacToeGrid grid)  {
		StringBuilder string = new StringBuilder();
		int i,j,k;
		int width = grid.getWidth();
		int pos = 0;
		int length = 1;
		String player = "";
		char p = '\0';
		Integer current = null;
		
		// first line
		string.append(_intersection);
		for (j = 0 ; j < width ; j++) {
			for (k = 0 ; k < _tileSize-1 ; k++)
				string.append(_horizontalSeparator);
			string.append(_intersection);
		}
		string.append('\n');

		// for each line
		for (i = 0 ; i < width ; i++) {
			string.append(_verticalSeparator);
			// for each column
			for (j = 0 ; j < width ; j++) {
				current = grid.get(i,j);
				if (current == TicTacToeGrid.FREE) {
					player = String.valueOf(j*width+i+1);
					length = player.length();
					p = '\0';
				}	
				else if (current == grid.P1) {
					length = 1;
					player = "\0";
					p = _token1;
				}
				else {
					length = 1;
					player = "";
					p = _token2;
				}
				pos = (_tileSize / 2) - (length / 2);
				for (k = 1 ; k < pos ; k++)
					string.append(_freeSpace);
				string.append(player);
				string.append(p);
				for (k = pos+length-1 ; k < _tileSize-1 ; k++)
					string.append(_freeSpace);
				string.append(_verticalSeparator);
			}
			string.append('\n');
			string.append(_intersection);
			for (j = 0 ; j < width ; j++ ) {
				for (k = 0 ; k < _tileSize - 1 ; k++)
					string.append(_horizontalSeparator);
				string.append(_intersection);
			}
			string.append('\n');
		}

		return string.toString();
	}

	char _token1 = 'X';
	char _token2 = 'O';
	char _verticalSeparator = '|';
	char _horizontalSeparator = '-';
	char _intersection = '+';
	int _tileSize = 4;
	char _freeSpace = ' ';

	/* PROMPT */

	public int menu() {
		try {
			return _scan.nextInt();
		}
		catch (Exception e) {
			displayError("Invalid input ! (int required)");
			return 0;
		}
	}

	public void newGame() {
		System.out.println(NEW_GAME);
		if (_source.getGrid() != null)
			_source.setGrid(new TicTacToeGrid(_source.getGrid().getWidth()));
	}

	public void gameOptions() {
		int nbPlayers;
		boolean playFirst;
		int width;
		// init players
		try {
		System.out.println(GAME_OPTIONS);
		System.out.println(ASK_NB_PLAYERS);
		nbPlayers = _scan.nextInt();
		switch(nbPlayers) {
			case 0 :
				_source.setP1(newAI(TicTacToeGrid.P1));
				_source.setP2(newAI(TicTacToeGrid.P2));
				break;
			case 1 :
				System.out.println(ASK_WHO_START);
				playFirst = _scan.nextBoolean();
				if (playFirst) {
					_source.setP1(newHuman());
					_source.setP2(newAI(TicTacToeGrid.P2));
				}
				else {
					_source.setP1(newAI(TicTacToeGrid.P1));
					_source.setP2(newHuman());
				}
				break;
			case 2 :
				_source.setP1(newHuman());
				_source.setP2(newHuman());
				break;
			default :		
				displayError(NB_PLAYERS_ERROR);
				throw new Exception();
		}
		// grid creation
		System.out.println(ASK_WIDTH);
		width = _scan.nextInt();
		if (width < 3) {
			displayError(WIDTH_ERROR);
			_source.setGrid(null);
		}
		else
			_source.setGrid(new TicTacToeGrid(width));
		}
		catch (Exception e) {
			displayError("Invalid input !");
		}
	}


	protected TicTacToePlayer newHuman() {
		System.out.println(HUMAN_CREATION);
		return new TicTacToeHuman();
	}

	protected TicTacToePlayer newAI(Integer p) {
		int maxDepth;
		boolean minimax;
		try {
		System.out.println(AI_CREATION);
		System.out.println(ASK_ALGORITHM);
		minimax = _scan.nextBoolean();
		System.out.println(ASK_DIFFICULTY);
		maxDepth = _scan.nextInt();
		return new TicTacToeAI(p,maxDepth,minimax);
		}
		catch (Exception e) {
			displayError("Invalid input !");
			return null;
		}
	}


	public void uiOptions() {
		System.out.println(UI_OPTIONS);
		System.out.println(ASK_TILE_SIZE);
		_tileSize = _scan.nextInt();
		System.out.println(ASK_TOKEN);
		_token1 = _scan.next().charAt(0);
		System.out.println(ASK_TOKEN);
		_token2 = _scan.next().charAt(0);
		System.out.println(ASK_VERTICAL_SEPARATOR);
		_verticalSeparator = _scan.next().charAt(0);
		System.out.println(ASK_HORIZONTAL_SEPARATOR);
		_horizontalSeparator = _scan.next().charAt(0);
		System.out.println(ASK_INTERSECTION);
		_intersection = _scan.next().charAt(0);
	
	}

	private Scanner _scan = new Scanner(System.in);

};

