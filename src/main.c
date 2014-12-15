#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit, atoi */
#include <getopt.h> /* getopt */
#include <assert.h>
#include <string.h> /* strlen */
#include "sudokulib.h"

#ifndef IMPLIES
#define IMPLIES(a, b) ((!(a)) || (b))
#endif

void print_versioninfo(void)
{
	printf("sudoku v0.2\n");
	printf("Usage: ./sudoku -h\n");
	printf("       ./sudoku -t {0..2}\n");
	printf("       ./sudoku [-v] -s 900000005:001920000:760500390:200608034:340090058:180305006:098006072:000072500:600000003\n");
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

int console_solve(const char *svalue, int vflag)
{
	state_t board;
	int i, j;
	const char *c;

	if (!svalue || strlen(svalue) != (N*N+(N-1))) {
		printf("invalid input length\n");
		return EXIT_FAILURE;
	}

	board = blank_state();
	
	/* parse input */
	c = svalue;
	i = 0; j = 0;
	while (i < N && j <= N) {
		if (*c == ':') { 
			++i; 
			j = 0;
		} else if ('1' <= *c && *c <= '9') {
			if (vflag) {
				printf("(%d,%d)->%d\n", i, j, *c-'1'+1);
			}
			set_given_disallow_others(&board, i, j, *c - '1' + 1);
			++j;
		} else {
			++j;
		}
		++c;
	}
	
	printf("Input:\n");
	print_state(&board);
	solve(&board);
	
	if (is_solved(&board)) {
		printf("Solved:\n");
	} else {
		printf("Not solved:\n");
	}
	print_state(&board);

	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	int vflag = 0;
	int tflag = 0;
	int hflag = 0;
	int sflag = 0;
	char *tvalue = NULL;
	char *svalue = NULL;
	int puzzle_no = 0;
	int c;

	while ((c = getopt(argc, argv, "vht:s:")) != -1) {
		switch (c) {
			case 'v': 
				vflag = 1; 
				break;
			case 'h':  /* help */
				hflag = 1;
				break;
			case 't':  /* test */
				tflag = 1; 
				tvalue = optarg;
				break;
			case 's':  /* solve */
				sflag = 1; 
				svalue = optarg;
				break;
			default:
				break;
		}
	}

	if (hflag) {
		print_versioninfo();
		exit(EXIT_SUCCESS);
	}

	if (tflag) {
		puzzle_no = atoi(tvalue);
		if (0 <= puzzle_no && puzzle_no <= 2) {
			printf("testing puzzle number: %d\n", puzzle_no);
			run_tests(puzzle_no);
		}
		exit(EXIT_SUCCESS);
	}

	if (sflag) {
		exit(console_solve(svalue, vflag));
	}

	return EXIT_SUCCESS;
}
