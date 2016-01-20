#include "sudoku.h"

int main(void) {
	srand(time(NULL));
	sudokuGrid game;
	int i;
	
	for (i = 0; i < 5; i++) {
		printf("\n\nBoard %i:\n",i);
		resetBoard(&game);
		generateUniqueBoard(&game);
		printBoard(&game);
	}
	
	while(1) { continue; }
	return 0;
}