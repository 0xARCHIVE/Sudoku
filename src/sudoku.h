#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define PRINCIPAL_NUM 3
#define NUM_VALUES (PRINCIPAL_NUM * PRINCIPAL_NUM)
#define GRID_SIZE (NUM_VALUES * NUM_VALUES)
#define MIN_VALUE 1
#define MAX_VALUE (MIN_VALUE + NUM_VALUES - 1)
#define BLANK_VALUE 0

typedef struct sudokuGrid { int values[GRID_SIZE]; } sudokuGrid;

int randInt(int min, int max);

void generateUniqueBoard(sudokuGrid *game);
int generateRandomBoard(sudokuGrid *game);
int isBoardFull(sudokuGrid *game);
int hasSolution(sudokuGrid *game);
int solutionCount(sudokuGrid *game, int solutionsCuttoff);
int isLegalMove(sudokuGrid *game, int position, int value);
void commitMove(sudokuGrid *game, int position, int value);
int getEmptyCell(sudokuGrid *game);
void printBoard(sudokuGrid *game);
void resetBoard(sudokuGrid *game);

void shuffleArr(int *array, size_t n);

int getCellRow(int position);
int getCellCol(int position);
int getBoxRow(int position);
int getBoxCol(int position);

#endif