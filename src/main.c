#include "sudoku.h"

int main(void) {
	srand(time(NULL));
	sudokuGrid game;
	/*int i;
	
	for (i = 0; i < 5; i++) {
		printf("\n\nBoard %i:\n",i);
		resetBoard(&game);
		generateUniqueBoard(&game);
		printBoard(&game);
	}*/
	
	char input;
	
	//generate the first game
	resetBoard(&game);
	generateUniqueBoard(&game);
	printBoard(&game);
	printf("\n\n(Press enter to generate a new Sudoku game)\n");
	
	while(1) {
		//search all inputted characters, look for a newline
		while (input = getchar()) {
			if (input == '\n') {
				//generate a new board
				resetBoard(&game);
				generateUniqueBoard(&game);
				printBoard(&game);
				printf("\n\n(Press enter to generate a new Sudoku game)\n");
			}
		}
	}
	return 0;
}