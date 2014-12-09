#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit, atoi */
#include <getopt.h> /* getopt */
#include <assert.h>
#include "sudokulib.h"

#ifndef IMPLIES
#define IMPLIES(a, b) ((!(a)) || (b))
#endif

void print_versioninfo(void)
{
	printf("sudoku v0.2\n");
	printf("author: Ivan Papusha\n");
	printf("board size: %lu bytes\n", sizeof(state_t));
#ifdef NDEBUG
	printf("debug: disabled\n");
#else
	printf("debug: enabled\n");
#endif
}

void run_tests(int puzzle_no)
{
	state_t board;
	int i, j;
	int npasses;

	board = sample_state(puzzle_no);
	printf("board:\n");
	print_state(&board);
	printf("\n");

	printf("num allowed:\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			printf("%d ", num_allowed(&board, i, j));
			assert( IMPLIES(board.val[i][j] == 0, num_allowed(&board, i, j) == 9) );
			assert( IMPLIES(board.val[i][j] != 0, num_allowed(&board, i, j) == 1) );
		}
		printf("\n");
	}
	printf("\n");
	
	npasses = simplify(&board);

	printf("board (after %d passes):\n", npasses);
	print_state(&board);
	printf("\n");

	printf("num allowed (after %d passes):\n", npasses);
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			printf("%d ", num_allowed(&board, i, j));
		}
		printf("\n");
	}

	printf("solving...");
	solve(&board);
	printf("done\n");

	if (is_solved(&board)) {
		printf("puzzle solved:\n");
		print_state(&board);
	} else {
		printf("puzzle not solved:\n");
		print_state(&board);
	}
}

int main(int argc, char **argv)
{
	int vflag = 0;
	int tflag = 0;
	char *tvalue = NULL;
	int puzzle_no = 0;
	int c;

	while ((c = getopt(argc, argv, "vt:")) != -1) {
		switch (c) {
			case 'v': 
				vflag = 1; 
				break;
			case 't': 
				tflag = 1; 
				tvalue = optarg;
				break;
			default:
				break;
		}
	}

	if (vflag) {
		print_versioninfo();
		exit(EXIT_SUCCESS);
	}

	if (tflag) {
		puzzle_no = atoi(tvalue);
		printf("testing puzzle number: %d\n", puzzle_no);
		run_tests(puzzle_no);
	}

	return EXIT_SUCCESS;
}
