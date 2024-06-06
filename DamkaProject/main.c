//Student1: Ido Tzror, Id: 211892427
//Student2: Rotem Lahav, Id: 322273335

#include "functions.h"

void main()
{
	Board board;

	initializeBoard(board);
	printBoard(board);

	PlayGame(board, TEAM_B);
}