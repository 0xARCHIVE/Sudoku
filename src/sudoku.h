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

void shuffleArr(int *array, size_t n) {
    if (n > 1) {
		int t;
        size_t i,j;
		for (i = 0; i < n - 1; i++) {
		  j = randInt(i,n-1);
		  t = array[j];
		  array[j] = array[i];
		  array[i] = t;
		}
    }
}

int randInt(int min, int max) {
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

int getCellRow(int position) {
	return (position/NUM_VALUES);
}

int getCellCol(int position) {
	return (position%NUM_VALUES);
}

int getBoxRow(int position) {
	return (getCellRow(position)/PRINCIPAL_NUM);
}

int getBoxCol(int position) {
	return (getCellCol(position)/PRINCIPAL_NUM);
}

int generateRandomBoard(sudokuGrid *game) {
	int position, trialValue, solved;
	int trialValues[NUM_VALUES];
	int i,j;
	
	//if the board is full, we are done
	if (isBoardFull(game)) {
		solved = TRUE;
	} else {
		//populate and then shuffle array of trial values
		j = 0;
		for (i = MIN_VALUE; i <= MAX_VALUE; i++) {
			trialValues[j] = i;
			j++;
		}
		shuffleArr(trialValues,NUM_VALUES);
		
		solved = FALSE;
		position = getEmptyCell(game);
		j = 0;
		
		while (!solved && (j < NUM_VALUES)) {
			trialValue = trialValues[j];
			
			if (isLegalMove(game,position,trialValue)) {
				commitMove(game,position,trialValue);
				
				if (generateRandomBoard(game)) {
					//game with trial value in this position is solvable
					solved = TRUE;
				} else {
					//reset to a blank value to try another solution
					commitMove(game,position,BLANK_VALUE);
				}
			}
			
			j++;
		}
	}
	
	return solved;
}


void generateUniqueBoard(sudokuGrid *game) {
	int i, j;
	int positions[GRID_SIZE];
	int targetValue, position;
	
	//start by generating a random board
	resetBoard(game);
	generateRandomBoard(game);
	
	//populate and shuffle positions array - these are targets for cell removal
	j = 0;
	for (i = 0; i <= GRID_SIZE; i++) {
		positions[j] = i;
		j++;
	}
	shuffleArr(positions,GRID_SIZE);
	
	//clear cells until we no longer have a unique solution
	j = 0;
	while (j < (GRID_SIZE - GRID_SIZE/PRINCIPAL_NUM)) {
		if (solutionCount(game,2) > 1) {
			//restore the last cell that we removed to ensure that we still have a unique solution
			commitMove(game,position,targetValue);
		}
		
		targetValue = game->values[positions[j]];
		position = positions[j];
		
		commitMove(game,position,BLANK_VALUE);
		j++;
	}
	
	//restore the last cell that we removed to ensure that we still have a unique solution
	commitMove(game,position,targetValue);
}

int hasSolution(sudokuGrid *game) {
	int position, trialValue, solved;
	
	//if the board is full, we are done
	if (isBoardFull(game)) {
		solved = TRUE;
	} else {
		solved = FALSE;
		position = getEmptyCell(game);
		trialValue = MIN_VALUE;
		
		while (!solved && (trialValue <= MAX_VALUE)) {
			if (isLegalMove(game,position,trialValue)) {
				commitMove(game,position,trialValue);
				
				if (hasSolution(game)) {
					//game with trial value in this position is solvable
					solved = TRUE;
				} else {
					//reset to a blank value to try another solution
					commitMove(game,position,BLANK_VALUE);
				}
			}
			
			trialValue++;
		}
	}
	
	return solved;
}

int solutionCount(sudokuGrid *game,int solutionsCuttoff) {
	//this function destroys the board in the process of counting the number of solutions//
	int position, trialValue, count;
	
	//if the board is full up, we are done
	if (isBoardFull(game)) {
		count = 1;
	} else {
		count = 0;
		
		position = getEmptyCell(game);
		trialValue = MIN_VALUE;
		while ((count < solutionsCuttoff) && (trialValue <= MAX_VALUE)) {
			if (isLegalMove(game,position,trialValue)) {
				commitMove(game,position,trialValue);
				if (solutionCount(game,1)) {
					//game with trial value in this position is solvable
					count++;
				}
				
				//reset to a blank value to try another solution
				commitMove(game,position,BLANK_VALUE);
			}
			
			trialValue++;
		}
	}
	
	return count;
}

int isLegalMove(sudokuGrid *game, int position, int value) {
	int cell_row,cell_col;
	int box_row,box_col;
	int i;
	
	//check if the position is valid
	if (position < 0 || position > GRID_SIZE) {
		return FALSE;
	}
	
	//check if the value is valid
	if (value < MIN_VALUE || value > MAX_VALUE) {
		return FALSE;
	}
	
	//check for clashes within the row
	cell_row = getCellRow(position);
	for (i = cell_row*NUM_VALUES; i < (cell_row + 1)*NUM_VALUES; i++) {
		if (game->values[i] == value) {
			return FALSE;
		}
	}
	
	//check for clashes within the column
	cell_col = getCellCol(position);
	for (i = cell_col; i < GRID_SIZE; i = i + NUM_VALUES) {
		if (game->values[i] == value) {
			return FALSE;
		}
	}
	
	//check for clashes within the box
	box_row = getBoxRow(position);
	box_col = getBoxCol(position);
	for (i = ((box_col*PRINCIPAL_NUM) + (box_row*PRINCIPAL_NUM*NUM_VALUES)); i < (((box_col*PRINCIPAL_NUM) + (box_row*PRINCIPAL_NUM*NUM_VALUES)) + ((NUM_VALUES*PRINCIPAL_NUM) - ((PRINCIPAL_NUM - 1)*PRINCIPAL_NUM))); i++) {
		if (i == position) { continue; }
		
		if (game->values[i] == value) {
			return FALSE;
		}
		
		if ((i+1)%PRINCIPAL_NUM == 0) { i = i + ((PRINCIPAL_NUM - 1)*PRINCIPAL_NUM); }
	}
	
	return TRUE;
}

void commitMove(sudokuGrid *game, int position, int value) {
	game->values[position] = value;
}

int isBoardFull(sudokuGrid *game) {
	if (getEmptyCell(game) == -1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int getEmptyCell(sudokuGrid *game) {
	int i = -1;
	int position;
	
	//traverse the game until you find an empty cell
	for (position = 0; position < GRID_SIZE; position++) {
		if (game->values[position] == BLANK_VALUE) {
			i = position;
			break;
		}
	}
	
	return i;
}

void printBoard(sudokuGrid *game) {
	int i = 0;
	for (i = 0; i < GRID_SIZE; i++) {
		if (i > 0) {
			if (i%PRINCIPAL_NUM == 0) {
				if (i%NUM_VALUES == 0) {
					printf("\n");
				} else {
					printf("\t");
				}
			}
			
			if (i%(PRINCIPAL_NUM*NUM_VALUES) == 0) {
				printf("\n");
			}
		}
		printf("%3i",game->values[i]);
	}
}

void resetBoard(sudokuGrid *game) {
	int i;
	for (i = 0; i < GRID_SIZE; i++) {
		game->values[i] = BLANK_VALUE;
	}
}