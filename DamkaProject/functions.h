#ifndef FUNCTION_H
#define FUNCTION_H

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define FULL_BOARD_SOLDIERS 12
#define TEAM_T 'T'
#define TEAM_B 'B'
#define EMPTY_SQUERE ' '
#define LAST_ROW 'H'
#define FIRST_COL '1'
#define SECOND_COL '2'
#define LAST_COL '8'
#define SEVENTH_COL '7'
#define SEVENTH_ROW 'G'
#define FIRST_ROW 'A'
#define SECOND_ROW 'B'

typedef struct _checksPos //משבצת
{
	char row;
	char col;
}checksPos;
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];
typedef unsigned char Player;
typedef struct _SingleSourceMovesTreeNode //עץ אפשרויות לצעדים
{
	Board board;
	checksPos* pos;
	unsigned short total_captures_so_far;
	struct _SingleSourceMovesTreeNode* next_move[2];
}SingleSourceMovesTreeNode;
typedef struct _SingleSourceMovesTree
{
	SingleSourceMovesTreeNode* source;
}SingleSourceMovesTree;
typedef struct _SingleSourceMovesListCell
{
	checksPos* position;
	unsigned short captures;
	struct _SingleSourceMovesListCell* next;
}SingleSourceMovesListCell;
typedef struct _SingleSourceMovesList
{
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
}SingleSourceMovesList;
typedef struct _multipleSourceMovesListCell
{
	SingleSourceMovesList* single_source_moves_list;
	struct _multipleSourceMovesListCell* next;
}MultipleSourceMovesListCell;
typedef struct _multipleSourceMovesList
{
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
}MultipleSourceMovesList;


//Qustion 1 functions
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checksPos* src);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(SingleSourceMovesTreeNode* root, Board board, checksPos* src, unsigned short eats, int countSteps);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(SingleSourceMovesTreeNode* root, Board board, checksPos* src, unsigned short eats, int countSteps);
bool checkifB(char point);
bool checkifT(char point);
bool ifSqureEmpty(char point);

//Tree functions
SingleSourceMovesTreeNode* createTreeNode(Board board, checksPos* pos, unsigned short captures);
SingleSourceMovesTree* createTree(Board board, checksPos* pos);
void freeTree(SingleSourceMovesTree* tr);
void freeTreeHelper(SingleSourceMovesTreeNode* root);

//Qustion 2 functions
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);
void FindSingleSourceOptimalMoveHelperT(SingleSourceMovesTreeNode* root, int* level, SingleSourceMovesList* lst);
void FindSingleSourceOptimalMoveHelperB(SingleSourceMovesTreeNode* root, int* level, SingleSourceMovesList* lst);
int findLevel(SingleSourceMovesTree* tr);
int findLevelHelper(SingleSourceMovesTreeNode* root);

//Qustion 3 functions
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);

//Qustion 4 function
void Turn(Board board, Player player);
MultipleSourceMovesListCell* findTheBestMove(MultipleSourceMovesList* allMoves, Player player);
bool findIfGameOver(MultipleSourceMovesList* allMoves, Player player, MultipleSourceMovesListCell** resNode);
void deleteEatenAndMovePlayer(SingleSourceMovesList* optimalMove, Player player, Board board);

//Question 5 functions
void PlayGame(Board board, Player starting_player);
void printWhosTurn(Player p);
bool isBInFirstLine(Board board);
bool isTInLastLine(Board board);

//list functions
SingleSourceMovesList* createList();
void makeEmptyList(SingleSourceMovesList* lst);
void insertDataToStartList(SingleSourceMovesList* lst, checksPos* pos, unsigned short captures);
SingleSourceMovesListCell* createNewListNode(checksPos* pos, unsigned short captures, SingleSourceMovesListCell* next);
void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head);
bool isEmptyList(SingleSourceMovesList* lst);

//list of list function
MultipleSourceMovesList* createListOfList();
void makeEmptyListOfList(MultipleSourceMovesList* lst);
void insertListDataToEndOfList(MultipleSourceMovesList* listOfList, SingleSourceMovesList* bestOptimalMove);
MultipleSourceMovesListCell* createNewListOfListNode(SingleSourceMovesList* bestOptimalMove, MultipleSourceMovesListCell* next);
void insertListNodeToEndOfList(MultipleSourceMovesList* listOfList, MultipleSourceMovesListCell* tail);
bool isEmptyListOfList(MultipleSourceMovesList* listOfList);
void freeListOfList(MultipleSourceMovesList* lst);


//board functions
void initializeBoard(Board board);
void printBoard(Board board);


#endif // !FUNCTION_H