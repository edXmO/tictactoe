#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// global constants
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

void displayInstructions();
char askYesNo(std::string question);
int askNumber(std::string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const std::vector<char>& board);
int humanMove(const std::vector<char>& board, char human);
bool isLegal(const std::vector<char>& board, int move);
int computerMove(const std::vector<char> board, char computer);
char winner(const std::vector<char>& board);
void announceWinner(char winner, char computer, char human);

int main()
{
	// Pseudocode
	// Create an empty tic-tac-toe board
	// display the game instructions
	// determine who goes first
	// display the board
	// while nobody has won and it snot a tie 
		// if it is human turn
			//get human move
			//update the board with the human move
		//otherwise
			// calculate computers move
			//update the board with the computers move accordingly
		//display the boards
		// switch turns
	//Congratulate the winner or declare a tie

	int move = 0;
	const int NUM_SQUARES = 9;
	std::vector<char> board(NUM_SQUARES, EMPTY);

	displayInstructions();
	char human = humanPiece();
	char computer = opponent(human);
	char turn = X;
	displayBoard(board);

	while (winner(board) == NO_ONE)
	{
		if (turn == human)
		{
			move = humanMove(board, human);
			board[move] = human;
		}
		else
		{
			move = computerMove(board, computer);
			board[move] = computer;
		}
		displayBoard(board);
		turn = opponent(turn);
	}

	announceWinner(winner(board), computer, human);

	return 0;
}

void displayInstructions()
{
	std::cout << "Bienvenido al Tres-En-Raya!\n";
	std::cout << "Haz tu movimiento introduciendo un numero del 0 al 8!\n";
	std::cout << "Ese numero correspondera con la posicion mostrada a continuacion.\n\n";

	std::cout << "      0 | 1 | 2\n";
	std::cout << "      ---------\n";
	std::cout << "      3 | 4 | 5\n";
	std::cout << "      ---------\n";
	std::cout << "      6 | 7 | 8\n\n";

	std::cout << "Preparate para jugar!\n\n";
}

char askYesNo(std::string question)
{
	char response = 'y';
	do
	{
		std::cout << question << "(s/n): ";
		std::cin >> response;
	} while (response != 's' && response != 'n');

	return response;
}

int askNumber(std::string question, int high, int low)
{
	int number = 0;
	do
	{
		std::cout << question << " (" << low << " - " << high << " ):";
		std::cin >> number;
	} while (number > high || number < low);

	return number;
}

char humanPiece()
{
	char go_first = askYesNo("Quieres empezar?");
	if (go_first == 's')
	{
		std::cout << "\nToma el primer movimiento.\n";
		return X;
	}
	else
	{
		std::cout << "\nEl ordenador (YO) empieza primero\n";
		return O;
	}
}

char opponent(char piece)
{
	if (piece == X)
	{
		return O;
	}
	else
	{
		return X;
	}
}

void displayBoard(const std::vector<char>& board)
{
	std::cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
	std::cout << "\n\t" << "----------";
	std::cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
	std::cout << "\n\t" << "----------";
	std::cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
	std::cout << "\n\n";
}

char winner(const std::vector<char>& board)
{
	// all possible winning rows
	const int WINNING_ROWS[8][3] = {
		{0,1,2},
		{3,4,5},
		{6,7,8},
		{0,3,6},
		{1,4,7},
		{2,5,8},
		{0,4,8},
		{2,4,6},
	};

	const int TOTAL_ROWS = 8;

	//if any winning row has three values that are the same(not empty)
	//then we ahve a winner

	for (int row = 0; row < TOTAL_ROWS; ++row)
	{
		if ((board[WINNING_ROWS[row][0]] != EMPTY) && (board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) && (board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
		{
			return board[WINNING_ROWS[row][0]];
		}
	}
	// if noone has won checking for tie
	if (std::count(board.begin(), board.end(), EMPTY) == 0)
	{
		return TIE;
	}

	return NO_ONE;
}

bool isLegal(const std::vector<char>& board, int move)
{
	return (board[move] == EMPTY);
}

int humanMove(const std::vector<char>& board, char human)
{
	int move = askNumber("Introduce la posicion donde quieres realizar el movimiento", (board.size() - 1));
	while (!isLegal(board, move))
	{
		std::cout << "\n Esa posicion ya se encuentra ocupada\n";
		move = askNumber("Introduce la posicion: ", (board.size() - 1));
	}
	return move;
}

int computerMove(std::vector<char> board, char computer)
{
	unsigned int move = 0;
	bool found = false;

	// if computer can win on next move, make move
	while (!found && move < board.size())
	{
		if (isLegal(board, move))
		{
			board[move] = computer;
			found = winner(board) == computer;
			board[move] = EMPTY;
		}

		if (!found)
		{
			++move;
		}
	}

	if (!found)
	{
		move = 0;
		char human = opponent(computer);

		while (!found && move < board.size())
		{
			if (isLegal(board, move))
			{
				board[move] = human;
				found = winner(board) == human;
				board[move] = EMPTY;
			}

			if (!found)
			{
				++move;
			}
		}
	}

	if (!found)
	{
		move = 0;
		const int BEST_MOVES[] = { 4, 0, 2 ,6 , 8, 1, 3 ,5, 7 };
		unsigned int i = 0;
		//pick best open square
		while (!found && i < board.size())
		{
			move = BEST_MOVES[i];
			if (isLegal(board, move))
			{
				found = true;
			}

			++i;
		}
	}

	std::cout << "Juego en la posicion " << move << std::endl;
	return move;
}

void announceWinner(char winner, char computer, char human)
{
	if (winner == computer)
	{
		std::cout << winner << " ha ganado!\n";
	}
	else if (winner == human)
	{
		std::cout << winner << " ha ganado!\n";
	}
	else {
		std::cout << "Es un empate.\n";
	}
}