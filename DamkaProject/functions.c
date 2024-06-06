#include "functions.h"

int tCount = FULL_BOARD_SOLDIERS;
int bCount = FULL_BOARD_SOLDIERS;
int mostCapInSingleMoveB = 0;
int mostCapInSingleMoveT = 0;


//Question1 functions
//the function create a binery tree with all the moves of the player
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checksPos* src)
{
	SingleSourceMovesTree* tr;
	tr = createTree(board, src);

	if (ifSqureEmpty(board[src->row - 'A'][src->col - '0' - 1])) //square is empty
		return NULL;
	else if (checkifT(board[src->row - 'A'][src->col - '0' - 1])) //if T
		tr->source = FindSingleSourceMovesHelperT(tr->source, board, src, 0, 0);
	else if (checkifB(board[src->row - 'A'][src->col - '0' - 1])) //if B
		tr->source = FindSingleSourceMovesHelperB(tr->source, board, src, 0, 0);

	return tr;
}
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(SingleSourceMovesTreeNode* root, Board board, checksPos* src, unsigned short eats, int countSteps)
{
	checksPos* temp = (checksPos*)malloc(sizeof(checksPos));
	if (temp == NULL)
	{
		printf("memory allocation failed");
		exit(1);
	}
	temp->col = src->col;
	temp->row = src->row;

	if (eats >= 1) //check in the recursion if the player has a capture
	{
		root = createTreeNode(board, src, eats);

		//check if the player has an option to eat from the right and not crossing the board's borders
		if (checkifB(board[src->row - 'A' + 1][src->col - '0']) && ifSqureEmpty(board[src->row - 'A' + 2][src->col - '0' + 1]) && (src->col != LAST_COL) && (src->row != LAST_ROW) && (src->row != SEVENTH_ROW) && (src->col != SEVENTH_COL))
		{
			src->row = src->row + 2; //update for the next position
			src->col = src->col + 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[1] = FindSingleSourceMovesHelperT(root->next_move[1], board, src, eats, countSteps);
			eats = eats - 1;
			src->row = temp->row; //update the player position
			src->col = temp->col;
		}
		else
			root->next_move[1] = NULL; //player cannot eat
		//check the same for the left side
		if (checkifB(board[src->row - 'A' + 1][src->col - '0' - 2]) && ifSqureEmpty(board[src->row - 'A' + 2][src->col - '0' - 3]) && (src->col != FIRST_COL) && (src->row != LAST_ROW) && (src->row != SEVENTH_ROW) && (src->col != SECOND_COL))
		{
			src->row = src->row + 2;
			src->col = src->col - 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[0] = FindSingleSourceMovesHelperT(root->next_move[0], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}
		else
			root->next_move[0] = NULL;
		free(temp);
		return root;
	}
	else if (eats == 0 && countSteps >= 1 || src->row == LAST_ROW) //stop conditions - the player did just one move
	{
		root = createTreeNode(board, src, eats);
		root->next_move[0] = root->next_move[1] = NULL;
		free(temp);
		return root;
	}
	else
	{
		//he can move to the left and to the right without eating
		if (ifSqureEmpty(board[src->row - 'A' + 1][src->col - '0']) && (src->col != LAST_COL))
		{
			src->row = src->row + 1;
			src->col = src->col + 1;
			countSteps = countSteps + 1;
			root->next_move[1] = FindSingleSourceMovesHelperT(root->next_move[1], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}
		if (ifSqureEmpty(board[src->row - 'A' + 1][src->col - '0' - 2]) && (src->col != FIRST_COL))
		{
			src->row = src->row + 1;
			src->col = src->col - 1;
			countSteps = countSteps + 1;
			root->next_move[0] = FindSingleSourceMovesHelperT(root->next_move[0], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}

		//he can move to the left and to the right with eating
		if (checkifB(board[src->row - 'A' + 1][src->col - '0']) && ifSqureEmpty(board[src->row - 'A' + 2][src->col - '0' + 1]) && (src->col != LAST_COL) && (src->row != LAST_ROW) && (src->row != SEVENTH_ROW) && (src->col != SEVENTH_COL))
		{
			src->row = src->row + 2;
			src->col = src->col + 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[1] = FindSingleSourceMovesHelperT(root->next_move[1], board, src, eats, countSteps);
			eats = eats - 1;
			src->row = temp->row;
			src->col = temp->col;
		}
		else if (!ifSqureEmpty(board[src->row - 'A' + 1][src->col - '0']))
			root->next_move[1] = NULL;
		if (checkifB(board[src->row - 'A' + 1][src->col - '0' - 2]) && ifSqureEmpty(board[src->row - 'A' + 2][src->col - '0' - 3]) && (src->col != FIRST_COL) && (src->row != LAST_ROW) && (src->row != SEVENTH_ROW) && (src->col != SECOND_COL))
		{
			src->row = src->row + 2;
			src->col = src->col - 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[0] = FindSingleSourceMovesHelperT(root->next_move[0], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}
		else if (!ifSqureEmpty(board[src->row - 'A' + 1][src->col - '0' - 2]))
			root->next_move[0] = NULL;
		free(temp);
		return root;
	}
}
//symmetrical to FindSingleSourceMovesHelperT
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(SingleSourceMovesTreeNode* root, Board board, checksPos* src, unsigned short eats, int countSteps)
{
	checksPos* temp = (checksPos*)malloc(sizeof(checksPos));
	if (temp == NULL)
	{
		printf("memory allocation failed");
		exit(1);
	}
	temp->col = src->col;
	temp->row = src->row;

	if (eats >= 1)
	{
		root = createTreeNode(board, src, eats);
		if (checkifT(board[src->row - 'A' - 1][src->col - '0']) && ifSqureEmpty(board[src->row - 'A' - 2][src->col - '0' + 1]) && (src->col != LAST_COL) && (src->row != FIRST_ROW) && (src->row != SECOND_ROW) && (src->col != SEVENTH_COL))
		{
			src->row = src->row - 2;
			src->col = src->col + 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[1] = FindSingleSourceMovesHelperB(root->next_move[1], board, src, eats, countSteps);
			eats = eats - 1;
			src->row = temp->row;
			src->col = temp->col;
		}
		else
			root->next_move[1] = NULL;
		if (checkifT(board[src->row - 'A' - 1][src->col - '0' - 2]) && ifSqureEmpty(board[src->row - 'A' - 2][src->col - '0' - 3]) && (src->col != FIRST_COL) && (src->row != FIRST_ROW) && (src->row != SECOND_ROW) && (src->col != SECOND_COL))
		{
			src->row = src->row - 2;
			src->col = src->col - 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[0] = FindSingleSourceMovesHelperB(root->next_move[0], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}
		else
			root->next_move[0] = NULL;
		free(temp);
		return root;
	}
	else if ((eats == 0 && countSteps >= 1) || src->row == FIRST_ROW)
	{
		root = createTreeNode(board, src, eats);
		root->next_move[0] = root->next_move[1] = NULL;
		free(temp);
		return root;
	}
	else
	{
		//he can move to the left and to the right without eating
		if (ifSqureEmpty(board[src->row - 'A' - 1][src->col - '0']) && (src->col != LAST_COL))
		{
			src->row = src->row - 1;
			src->col = src->col + 1;
			countSteps = countSteps + 1;
			root->next_move[1] = FindSingleSourceMovesHelperB(root->next_move[1], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}
		if (ifSqureEmpty(board[src->row - 'A' - 1][src->col - '0' - 2]) && (src->col != FIRST_COL))
		{
			src->row = src->row - 1;
			src->col = src->col - 1;
			countSteps = countSteps + 1;
			root->next_move[0] = FindSingleSourceMovesHelperB(root->next_move[0], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}

		//he can move to the left and to the right with eating
		if (checkifT(board[src->row - 'A' - 1][src->col - '0']) && ifSqureEmpty(board[src->row - 'A' - 2][src->col - '0' + 1]) && (src->col != LAST_COL) && (src->row != FIRST_ROW) && (src->row != SECOND_ROW) && (src->col != SEVENTH_COL))
		{
			src->row = src->row - 2;
			src->col = src->col + 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[1] = FindSingleSourceMovesHelperB(root->next_move[1], board, src, eats, countSteps);
			eats = eats - 1;
			src->row = temp->row;
			src->col = temp->col;
		}
		else if (!ifSqureEmpty(board[src->row - 'A' - 1][src->col - '0']))
			root->next_move[1] = NULL;
		if (checkifT(board[src->row - 'A' - 1][src->col - '0' - 2]) && ifSqureEmpty(board[src->row - 'A' - 2][src->col - '0' - 3]) && (src->col != FIRST_COL) && (src->row != FIRST_ROW) && (src->row != SECOND_ROW) && (src->col != SECOND_COL))
		{
			src->row = src->row - 2;
			src->col = src->col - 2;
			countSteps = countSteps + 1;
			eats = eats + 1;
			root->next_move[0] = FindSingleSourceMovesHelperB(root->next_move[0], board, src, eats, countSteps);
			src->row = temp->row;
			src->col = temp->col;
		}
		else if (!ifSqureEmpty(board[src->row - 'A' - 1][src->col - '0' - 2]))
			root->next_move[0] = NULL;
		free(temp);
		return root;
	}
}
//check if the player is from team B
bool checkifB(char point)
{
	if (point == TEAM_B)
		return true;
	else
		return false;
}
//check if the player is from team T
bool checkifT(char point)
{
	if (point == TEAM_T)
		return true;
	else
		return false;
}
//check if there is no player on the square
bool ifSqureEmpty(char point)
{
	if (point != TEAM_T && point != TEAM_B)
		return true;
	else
		return false;
}

// Tree functions
// creating a tree node
SingleSourceMovesTreeNode* createTreeNode(Board board, checksPos* pos, unsigned short captures)
{
	int i, j;
	SingleSourceMovesTreeNode* res;
	res = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	if (!res)
	{
		printf("mamorey allocation failed!");
		exit(1);
	}
	res->pos = (checksPos*)malloc(sizeof(checksPos));
	if (!(res->pos))
	{
		printf("mamorey allocation failed!");
		exit(1);
	}
	*(res->pos) = *pos;
	res->total_captures_so_far = captures;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
			res->board[i][j] = board[i][j];
	}
	res->next_move[0] = res->next_move[1] = NULL;
	return res;
}
// creating a tree and creating a source to the tree
SingleSourceMovesTree* createTree(Board board, checksPos* pos)
{
	SingleSourceMovesTree* tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	if (tree == NULL)
	{
		printf("memorey allocation failed");
		exit(1);
	}

	tree->source = createTreeNode(board, pos, 0);

	return tree;
}
// freeing Tree
void freeTree(SingleSourceMovesTree* tr)
{
	return freeTreeHelper(tr->source);
}
void freeTreeHelper(SingleSourceMovesTreeNode* root)
{
	if (root == NULL)
		return;
	else
	{
		freeTreeHelper(root->next_move[0]);
		freeTreeHelper(root->next_move[1]);
		free(root->pos);
		free(root);

		return;
	}
}

//Question2 functions
//the function get a tree and return a list of the route that contains the most captures
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree)
{
	int biggestLevel;
	bool resIn = true;
	int i = moves_tree->source->pos->row - 'A';
	int j = moves_tree->source->pos->col - '0' - 1;
	SingleSourceMovesList* lst;
	lst = createList();
	makeEmptyList(lst);
	biggestLevel = findLevel(moves_tree); //finding the level of the tree
	if (moves_tree->source->next_move[0] == NULL && moves_tree->source->next_move[1] != NULL) //right child
	{
		if (moves_tree->source->next_move[1]->total_captures_so_far == 0) //without eating
		{
			insertDataToStartList(lst, moves_tree->source->next_move[1]->pos, moves_tree->source->next_move[1]->total_captures_so_far);
			insertDataToStartList(lst, moves_tree->source->pos, moves_tree->source->total_captures_so_far);
			resIn = false;
		}
	}
	if (moves_tree->source->next_move[1] == NULL && moves_tree->source->next_move[0] != NULL && resIn) //left child
	{
		if (moves_tree->source->next_move[0]->total_captures_so_far == 0) //without eating
		{
			insertDataToStartList(lst, moves_tree->source->next_move[0]->pos, moves_tree->source->next_move[0]->total_captures_so_far);
			insertDataToStartList(lst, moves_tree->source->pos, moves_tree->source->total_captures_so_far);
			resIn = false;
		}
	}
	if (moves_tree->source->next_move[0] != NULL && moves_tree->source->next_move[1] != NULL && resIn) //has childs in left and right
	{
		if (moves_tree->source->next_move[0]->total_captures_so_far == 0 && moves_tree->source->next_move[1]->total_captures_so_far == 0) //the childs have no captures
		{
			if (checkifT(moves_tree->source->board[i][j]))
			{
				insertDataToStartList(lst, moves_tree->source->next_move[1]->pos, moves_tree->source->next_move[1]->total_captures_so_far);
				insertDataToStartList(lst, moves_tree->source->pos, moves_tree->source->total_captures_so_far);
				resIn = false;
			}
			else
			{
				insertDataToStartList(lst, moves_tree->source->next_move[0]->pos, moves_tree->source->next_move[0]->total_captures_so_far);
				insertDataToStartList(lst, moves_tree->source->pos, moves_tree->source->total_captures_so_far);
				resIn = false;
			}
		}
		else if (moves_tree->source->next_move[0]->total_captures_so_far == 0 && moves_tree->source->next_move[1]->total_captures_so_far != 0)
			moves_tree->source->next_move[0] = NULL;
		else if (moves_tree->source->next_move[0]->total_captures_so_far != 0 && moves_tree->source->next_move[1]->total_captures_so_far == 0)
			moves_tree->source->next_move[1] = NULL;
	}
	if (moves_tree->source->board[i][j] == TEAM_T && resIn)
		FindSingleSourceOptimalMoveHelperT(moves_tree->source, &biggestLevel, lst);
	if (moves_tree->source->board[i][j] == TEAM_B && resIn)
		FindSingleSourceOptimalMoveHelperB(moves_tree->source, &biggestLevel, lst);
	return lst;
}
void FindSingleSourceOptimalMoveHelperT(SingleSourceMovesTreeNode* root, int* level, SingleSourceMovesList* lst)
{
	if (root == NULL)
		return;
	else
	{
		FindSingleSourceOptimalMoveHelperT(root->next_move[1], level, lst);
		FindSingleSourceOptimalMoveHelperT(root->next_move[0], level, lst);
		if (root->total_captures_so_far == *level) //compare between the captures and the level
		{
			insertDataToStartList(lst, root->pos, root->total_captures_so_far); //insert the node to the list
			*level = *level - 1; //update level
		}
	}
}
//symmetrical to FindSingleSourceOptimalMoveHelperT
void FindSingleSourceOptimalMoveHelperB(SingleSourceMovesTreeNode* root, int* level, SingleSourceMovesList* lst)
{
	if (root == NULL)
		return;
	else
	{
		FindSingleSourceOptimalMoveHelperB(root->next_move[0], level, lst);
		FindSingleSourceOptimalMoveHelperB(root->next_move[1], level, lst);
		if (root->total_captures_so_far == *level)
		{
			insertDataToStartList(lst, root->pos, root->total_captures_so_far);
			*level = *level - 1;
		}
	}
}
//finding the level of the tree
int findLevel(SingleSourceMovesTree* tr)
{
	return findLevelHelper(tr->source);
}
int findLevelHelper(SingleSourceMovesTreeNode* root)
{
	int levelLeft, levelRight;
	if (root == NULL)
		return -1;
	else
	{
		levelLeft = findLevelHelper(root->next_move[0]);
		levelRight = findLevelHelper(root->next_move[1]);
		if (levelLeft >= levelRight)
			return levelLeft + 1;
		else
			return levelRight + 1;
	}
}

//Question 3 functions
//the function returns a list with a lists of the best route of each player
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{
	MultipleSourceMovesList* listOfList = createListOfList();
	SingleSourceMovesTree* tr;
	SingleSourceMovesList* bestOptimalMove;
	checksPos* src = (checksPos*)malloc(sizeof(checksPos));
	if (!src)
	{
		printf("mamorey allocation failed!");
		exit(1);
	}
	int i, j;
	makeEmptyListOfList(listOfList);

	//go over all of the players
	for (i = 0; i < BOARD_SIZE; i++)
	{
		if (i % 2 == 0)
			j = 1;
		else
			j = 0;

		for (j = j; j < BOARD_SIZE; j = j + 2)
		{
			if (board[i][j] == player)
			{
				src->row = i + 'A';
				src->col = j + '0' + 1;
				tr = FindSingleSourceMoves(board, src); //create the tree of the player
				bestOptimalMove = FindSingleSourceOptimalMove(tr); //finding the best route
				insertListDataToEndOfList(listOfList, bestOptimalMove); //put ithe best route in the list
			}
		}
	}
	return listOfList;
}
MultipleSourceMovesList* createListOfList()
{
	MultipleSourceMovesList* lst;
	lst = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	if (!lst)
	{
		printf("mamorey allocation failed!");
		exit(1);
	}
	return lst;
}
void makeEmptyListOfList(MultipleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
	return;
}
void insertListDataToEndOfList(MultipleSourceMovesList* listOfList, SingleSourceMovesList* bestOptimalMove)
{
	MultipleSourceMovesListCell* newTail;
	newTail = createNewListOfListNode(bestOptimalMove, NULL);
	insertListNodeToEndOfList(listOfList, newTail);
}
MultipleSourceMovesListCell* createNewListOfListNode(SingleSourceMovesList* bestOptimalMove, MultipleSourceMovesListCell* next)
{
	MultipleSourceMovesListCell* res;
	res = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	if (!res)
	{
		printf("mamorey allocation failed!");
		exit(1);
	}
	res->single_source_moves_list = bestOptimalMove;
	res->next = next;
	return res;
}
void insertListNodeToEndOfList(MultipleSourceMovesList* listOfList, MultipleSourceMovesListCell* tail)
{
	if (isEmptyListOfList(listOfList) == true)
		listOfList->head = listOfList->tail = tail;
	else
	{
		listOfList->tail->next = tail;
		listOfList->tail = tail;
	}
	tail->next = NULL;
}
bool isEmptyListOfList(MultipleSourceMovesList* listOfList)
{
	if (listOfList->head == NULL)
		return true;
	return false;
}


//Question 4 functions
//the function plays the best move
void Turn(Board board, Player player)
{
	MultipleSourceMovesList* allMoves = FindAllPossiblePlayerMoves(board, player);
	MultipleSourceMovesListCell* optimalMove = findTheBestMove(allMoves, player); //finding the optimal move

	if (player == TEAM_T)
	{
		tCount -= optimalMove->single_source_moves_list->tail->captures; //decrease the amount of T players
		if (optimalMove->single_source_moves_list->tail->captures >= 0)
			mostCapInSingleMoveT = optimalMove->single_source_moves_list->tail->captures; //update the move with the max capures
	}
	else
	{
		bCount -= optimalMove->single_source_moves_list->tail->captures; //decrease the amount of B players
		if (optimalMove->single_source_moves_list->tail->captures >= 0)
			mostCapInSingleMoveB = optimalMove->single_source_moves_list->tail->captures; //update the move with the max capures
	}

	printf("%c%c->%c%c\n", optimalMove->single_source_moves_list->head->position->row, optimalMove->single_source_moves_list->head->position->col, optimalMove->single_source_moves_list->tail->position->row, optimalMove->single_source_moves_list->tail->position->col);
	deleteEatenAndMovePlayer(optimalMove->single_source_moves_list, player, board); //update the board
	freeListOfList(allMoves);
}
//the function prioritized what is the optimal move according to the terms
MultipleSourceMovesListCell* findTheBestMove(MultipleSourceMovesList* allMoves, Player player)
{
	MultipleSourceMovesListCell* currRead, * currWrite, * res;
	currWrite = allMoves->head;
	currRead = currWrite->next;

	if (findIfGameOver(allMoves, player, &res))
		return res;

	while (currRead != NULL && currWrite != NULL) //read/write algoritem
	{
		//the player is stuck and cannot move
		if (player == TEAM_B && currWrite->single_source_moves_list->head->next == NULL)
		{
			currWrite = currWrite->next;
			currRead = currRead->next;
		}
		if (player == TEAM_T && currRead->single_source_moves_list->head->next == NULL)
		{
			currRead = currRead->next;
		}
		//prioritize according to the captures
		else if (currWrite->single_source_moves_list->tail->captures > currRead->single_source_moves_list->tail->captures)
			currRead = currRead->next;
		else if (currWrite->single_source_moves_list->tail->captures < currRead->single_source_moves_list->tail->captures)
		{
			currWrite = currRead;
			currRead = currRead->next;
		}
		else
		{
			if (player == TEAM_B)
			{
				//prioritize according to the rows of B
				if (currWrite->single_source_moves_list->head->position->row < currRead->single_source_moves_list->head->position->row)
					currRead = currRead->next;
				else if (currWrite->single_source_moves_list->head->position->row > currRead->single_source_moves_list->head->position->row)
				{
					currWrite = currRead;
					currRead = currRead->next;
				}
				else
				{
					//prioritize according to the cols of B
					if (currWrite->single_source_moves_list->head->position->col < currRead->single_source_moves_list->head->position->col)
						currRead = currRead->next;
					else if (currWrite->single_source_moves_list->head->position->col > currRead->single_source_moves_list->head->position->col)
					{
						currWrite = currRead;
						currRead = currRead->next;
					}
				}
			}
			if (player == TEAM_T)
			{
				//prioritize according to the rows of T
				if (currWrite->single_source_moves_list->head->position->row > currRead->single_source_moves_list->head->position->row)
					currRead = currRead->next;
				else if (currWrite->single_source_moves_list->head->position->row < currRead->single_source_moves_list->head->position->row)
				{
					currWrite = currRead;
					currRead = currRead->next;
				}
				else
				{
					//prioritize according to the cols of T
					if (currWrite->single_source_moves_list->head->position->col > currRead->single_source_moves_list->head->position->col)
						currRead = currRead->next;
					else if (currWrite->single_source_moves_list->head->position->col < currRead->single_source_moves_list->head->position->col)
					{
						currWrite = currRead;
						currRead = currRead->next;
					}
				}
			}
		}
	}
	return currWrite;
}
//the function check if the game is over after the move and prioritized this specific move
bool findIfGameOver(MultipleSourceMovesList* allMoves, Player player, MultipleSourceMovesListCell** resNode)
{
	MultipleSourceMovesListCell* currRead, * currWrite;
	currWrite = allMoves->head;
	currRead = currWrite->next;
	bool res = false;

	while (currRead != NULL && currWrite != NULL) //read/write algoritem
	{
		if (player == TEAM_T)
		{
			//check according to the last row for T
			if ((currWrite->single_source_moves_list->tail->position->row != LAST_ROW && currRead->single_source_moves_list->tail->position->row != LAST_ROW))
				currRead = currRead->next;
			else if (currWrite->single_source_moves_list->tail->position->row == LAST_ROW && currRead->single_source_moves_list->tail->position->row != LAST_ROW)
			{
				currRead = currRead->next;
				res = true;
			}
			else if (currWrite->single_source_moves_list->tail->position->row != LAST_ROW && currRead->single_source_moves_list->tail->position->row == LAST_ROW)
			{
				currWrite = currRead;
				currRead = currRead->next;
				res = true;
			}
			else
			{
				//check for the route that end in the board's last row with the most captures and prioritized it
				if (currWrite->single_source_moves_list->tail->captures > currRead->single_source_moves_list->tail->captures)
					currRead = currRead->next;
				else if (currWrite->single_source_moves_list->tail->captures < currRead->single_source_moves_list->tail->captures)
				{
					currWrite = currRead;
					currRead = currRead->next;
				}
				else
				{
					if (currWrite->single_source_moves_list->head->position->row > currRead->single_source_moves_list->head->position->row)
						currRead = currRead->next;
					else if (currWrite->single_source_moves_list->head->position->row < currRead->single_source_moves_list->head->position->row)
					{
						currWrite = currRead;
						currRead = currRead->next;
					}
					else
					{
						if (currWrite->single_source_moves_list->head->position->col > currRead->single_source_moves_list->head->position->col)
							currRead = currRead->next;
						else if (currWrite->single_source_moves_list->head->position->col < currRead->single_source_moves_list->head->position->col)
						{
							currWrite = currRead;
							currRead = currRead->next;
						}
					}
				}
			}
		}
		else
		{
			//check according to the last row for B
			if ((currWrite->single_source_moves_list->tail->position->row != FIRST_ROW && currRead->single_source_moves_list->tail->position->row != FIRST_ROW))
				currRead = currRead->next;
			else if (currWrite->single_source_moves_list->tail->position->row == FIRST_ROW && currRead->single_source_moves_list->tail->position->row != FIRST_ROW)
			{
				currRead = currRead->next;
				res = true;
			}
			else if (currWrite->single_source_moves_list->tail->position->row != FIRST_ROW && currRead->single_source_moves_list->tail->position->row == FIRST_ROW)
			{
				currWrite = currRead;
				currRead = currRead->next;
				res = true;
			}
			else
			{
				//check for the route that end in the board's last row with the most captures and prioritized it
				if (currWrite->single_source_moves_list->tail->captures > currRead->single_source_moves_list->tail->captures)
					currRead = currRead->next;
				else if (currWrite->single_source_moves_list->tail->captures < currRead->single_source_moves_list->tail->captures)
				{
					currWrite = currRead;
					currRead = currRead->next;
				}
				else
				{
					if (currWrite->single_source_moves_list->head->position->row < currRead->single_source_moves_list->head->position->row)
						currRead = currRead->next;
					else if (currWrite->single_source_moves_list->head->position->row > currRead->single_source_moves_list->head->position->row)
					{
						currWrite = currRead;
						currRead = currRead->next;
					}
					else
					{
						if (currWrite->single_source_moves_list->head->position->col < currRead->single_source_moves_list->head->position->col)
							currRead = currRead->next;
						else if (currWrite->single_source_moves_list->head->position->col > currRead->single_source_moves_list->head->position->col)
						{
							currWrite = currRead;
							currRead = currRead->next;
						}
					}
				}
			}
		}
	}
	if (res == true)
		*resNode = currWrite;
	return res;
}
//the function updates the board
void deleteEatenAndMovePlayer(SingleSourceMovesList* optimalMove, Player player, Board board)
{
	SingleSourceMovesListCell* curr = optimalMove->head;

	while (curr->next != NULL)
	{
		if (player == TEAM_T)
		{
			if (optimalMove->tail->captures != 0)
			{
				board[curr->next->position->row - 'A'][curr->next->position->col - '0' - 1] = TEAM_T;
				if (curr->position->col - '0' - 1 > curr->next->position->col - '0' - 1)
					board[curr->position->row - 'A' + 1][curr->position->col - '0' - 2] = EMPTY_SQUERE;
				else
					board[curr->position->row - 'A' + 1][curr->position->col - '0'] = EMPTY_SQUERE;
				board[curr->position->row - 'A'][curr->position->col - '0' - 1] = EMPTY_SQUERE;
			}
			else
			{
				board[curr->next->position->row - 'A'][curr->next->position->col - '0' - 1] = TEAM_T;
				board[curr->position->row - 'A'][curr->position->col - '0' - 1] = EMPTY_SQUERE;
			}
		}
		else
		{
			if (optimalMove->tail->captures != 0)
			{
				board[curr->next->position->row - 'A'][curr->next->position->col - '0' - 1] = TEAM_B;
				if (curr->position->col - '0' - 1 > curr->next->position->col - '0' - 1)
					board[curr->position->row - 'A' - 1][curr->position->col - '0' - 2] = EMPTY_SQUERE;
				else
					board[curr->position->row - 'A' - 1][curr->position->col - '0'] = EMPTY_SQUERE;
				board[curr->position->row - 'A'][curr->position->col - '0' - 1] = EMPTY_SQUERE;
			}
			else
			{
				board[curr->next->position->row - 'A'][curr->next->position->col - '0' - 1] = TEAM_B;
				board[curr->position->row - 'A'][curr->position->col - '0' - 1] = EMPTY_SQUERE;
			}
		}
		curr = curr->next;
	}
}

void freeListOfList(MultipleSourceMovesList* lst)
{
	MultipleSourceMovesListCell* curr, * temp;
	SingleSourceMovesListCell* currLittle, * tempLittle;
	curr = temp = lst->head;
	currLittle = tempLittle = curr->single_source_moves_list->head;

	while (curr != NULL)
	{
		temp = curr;
		curr = curr->next;
		while (currLittle != NULL)
		{
			tempLittle = currLittle;
			currLittle = currLittle->next;
			free(tempLittle);

		}
		free(temp);
	}
	return;
}



//Question 5 functions
//the function plays the game
void PlayGame(Board board, Player starting_player)
{
	int movesT = 0, movesB = 0;
	while ((bCount != 0) && (tCount != 0) && !isBInFirstLine(board) && !isTInLastLine(board))
	{
		printWhosTurn(starting_player);
		Turn(board, starting_player); //plays the turn
		if (starting_player == TEAM_T)
		{
			starting_player = TEAM_B; //replace to B's turn
			movesT++; //counting the moves of T
		}
		else
		{
			starting_player = TEAM_T; //replace to T's turn
			movesB++; //counting the moves of B
		}
		printBoard(board);
		printf("\n");
	}
	if (isBInFirstLine(board) || tCount == 0) //B wins
	{
		printf("B wins!\n");
		printf("B performed %d moves\n", movesB);
	}
	if (isTInLastLine(board) || bCount == 0) //T wins
	{
		printf("T wins!\n");
		printf("T performed %d moves\n", movesT);
	}
	if (movesB > movesT)
		printf("B performed the highest number of captures in a single move - %d\n", mostCapInSingleMoveB);
	else
		printf("T performed the highest number of captures in a single move - %d\n", mostCapInSingleMoveT);

}
void printWhosTurn(Player p)
{
	if (p == TEAM_T)
		printf("T's turn:\n");
	else
		printf("B's turn:\n");
}
//check if B got to the first line
bool isBInFirstLine(Board board)
{
	int j;
	for (j = 0; j < BOARD_SIZE; j++)
	{
		if (board[0][j] == TEAM_B)
			return true;
	}
	return false;
}
//check if T got to the last line
bool isTInLastLine(Board board)
{
	int j;
	for (j = 0; j < BOARD_SIZE; j++)
	{
		if (board[7][j] == TEAM_T)
			return true;
	}
	return false;
}

//list functions
SingleSourceMovesList* createList()
{
	SingleSourceMovesList* lst;
	lst = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	if (!lst)
	{
		printf("mamorey allocation failed!");
		exit(1);
	}
	return lst;
}
void makeEmptyList(SingleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}
void insertDataToStartList(SingleSourceMovesList* lst, checksPos* pos, unsigned short captures)
{
	SingleSourceMovesListCell* newHead;
	newHead = createNewListNode(pos, captures, NULL);
	insertNodeToStartList(lst, newHead);
}
SingleSourceMovesListCell* createNewListNode(checksPos* pos, unsigned short captures, SingleSourceMovesListCell* next)
{
	SingleSourceMovesListCell* res;
	res = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	if (!res)
	{
		printf("memory allocation failed!!");
		exit(1);
	}
	res->captures = captures;
	res->position = pos;
	res->next = next;
	return res;
}
void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head)
{
	if (isEmptyList(lst) == true)
	{
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else
	{
		head->next = lst->head;
		lst->head = head;
	}
}
bool isEmptyList(SingleSourceMovesList* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}

//board functions
void initializeBoard(Board board)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (i < 3 && (i + j) % 2 == 1)
				board[i][j] = 'T'; // Top player's piece
			else if (i > 4 && (i + j) % 2 == 1)
				board[i][j] = 'B'; // Bottom player's piece
			else
				board[i][j] = ' '; // Empty square
		}
	}

	//for (int row = 0; row < BOARD_SIZE; row++)
	//{
	//	for (int col = 0; col < BOARD_SIZE; col++)
	//	{
	//		board[row][col] = ' ';
	//	}
	//}


	//// Place initial pieces after a few moves
	//board[0][1] = 'T';
	//board[0][3] = 'T';
	//board[0][5] = 'T';
	//board[0][7] = 'T';
	//board[1][0] = 'T';
	//board[1][2] = 'T';
	//board[1][4] = 'T';
	//board[1][6] = 'T';
	//board[2][1] = 'T';
	//board[2][3] = 'T';
	//board[2][5] = 'T';
	//board[4][7] = 'T';

	//board[7][0] = 'B';
	//board[7][2] = 'B';
	//board[7][4] = 'B';
	//board[7][6] = 'B';
	//board[6][1] = 'B';
	//board[6][3] = 'B';
	//board[6][5] = 'B';
	//board[6][7] = 'B';
	//board[3][0] = 'B';
	//board[5][2] = 'B';
	//board[5][4] = 'B';
	//board[5][6] = 'B';
}
void printBoard(Board gameBoard)
{
	int i, j;
	char row = 'A';
	printf("+-+-+-+-+-+-+-+-+-+\n");
	printf("+ |1|2|3|4|5|6|7|8|\n");
	printf("+-+-+-+-+-+-+-+-+-+\n");
	for (i = 0; i < BOARD_SIZE; i++)
	{
		printf("|%c|", row + i);

		for (j = 0; j < BOARD_SIZE; j++)
		{
			printf("%c|", gameBoard[i][j]);
		}
		printf("\n+-+-+-+-+-+-+-+-+-+\n");
	}
}