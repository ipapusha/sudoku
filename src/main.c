#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit */
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

void run_tests(void)
{
	state_t board0;
	int i, j;
	int npasses;

	board0 = sample_state(1);
	printf("board:\n");
	print_state(&board0);
	printf("\n");

	printf("num allowed:\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			printf("%d ", num_allowed(&board0, i, j));
			assert( IMPLIES(board0.val[i][j] == 0, num_allowed(&board0, i, j) == 9) );
			assert( IMPLIES(board0.val[i][j] != 0, num_allowed(&board0, i, j) == 1) );
		}
		printf("\n");
	}
	printf("\n");
	
	npasses = simplify(&board0);

	printf("board (after %d passes):\n", npasses);
	print_state(&board0);
	printf("\n");

	printf("num allowed (after %d passes):\n", npasses);
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			printf("%d ", num_allowed(&board0, i, j));
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int vflag = 0;
	int tflag = 0;
	int c;

	while ((c = getopt(argc, argv, "vt")) != -1) {
		switch (c) {
			case 'v': 
				vflag = 1; 
				break;
			case 't': 
				tflag = 1; 
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
		run_tests();
	}

	return EXIT_SUCCESS;
}
