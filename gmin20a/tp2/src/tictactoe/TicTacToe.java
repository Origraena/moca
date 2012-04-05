package tictactoe;

public class TicTacToe {

	public static final int NEW_GAME = 1;
	public static final int GAME_OPTIONS = 2;
	public static final int UI_OPTIONS = 3;
	public static final int EXIT = 4;

	public TicTacToe() {
		_ui = new TicTacToeSUI(this);
	}

	public TicTacToe(UI userInterface) {
		_ui = userInterface;
	}

	public TicTacToe(TicTacToePlayer p1, TicTacToePlayer p2, int width) {
		_p1 = p1;
		_p2 = p2;
		_grid = new TicTacToeGrid(width);
		_ui = new TicTacToeSUI(this);
		_currentPlayer = _p1;
	}

	public void run() {
		_isRunning = true;
		int selection = 0;
		while (_isRunning) {
			_ui.displayMenu();
			selection = _ui.menu();
			switch (selection) {
				case NEW_GAME :
					_ui.newGame();
					if (isValid()) {
						runGame();
						break;
					}
					else 
						_ui.displayError("Non valid game, going to options menu...");
				case GAME_OPTIONS :
					_ui.gameOptions();
					break;
				case UI_OPTIONS :
					_ui.uiOptions();
					break;
				case EXIT :
					_isRunning = false;
					break;
				default :
					_ui.displayError("Selection error. Try again.");
					break;
			}
		}
	}

	protected boolean isValid() {
		return !((_p1 == null) || (_p2 == null) || (_grid == null)); 
	}

	protected void runGame() {
		_ui.displayGrid();
		_currentPlayer = _p1;
		_p1.init();
		_p2.init();
		while (!_grid.hasEnded()) {
			_grid = _currentPlayer.play(_grid);
			if (_currentPlayer == _p1)
				_currentPlayer = _p2;
			else
				_currentPlayer = _p1;
			_ui.displayGrid();
		}
		_ui.displayWinner();
	}

	/* package-ranged methods */
	void setP1(TicTacToePlayer value) {
		_p1 = value;
	}
	void setP2(TicTacToePlayer value) {
		_p2 = value;
	}
	void setGrid(TicTacToeGrid value) {
		_grid = value;
	}
	TicTacToeGrid getGrid() {
		return _grid;
	}

	TicTacToePlayer _p1;
	TicTacToePlayer _p2;
	TicTacToePlayer _currentPlayer;
	UI _ui;
	TicTacToeGrid _grid;

	boolean _isRunning = false;

	public static interface UI {

		/* display methods */
		public void displayGrid();
		public void displayMenu();
		public void displayWinner();
		public void displayError(String error);

		/* prompt methods */
		public int menu();
		public void newGame();
		public void gameOptions();
		public void uiOptions();

	}

};

