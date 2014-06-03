#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit, malloc */
#include <assert.h>
#include <unistd.h> /* getopt */
#include <getopt.h>
#include <inttypes.h>
#include "sudokulib.h"

void print_versioninfo(void)
{
	printf("sudoku v0.1\n");
	printf("Author: Ivan Papusha\n");
#ifdef NDEBUG
	printf("debug disabled\n");
#else
	printf("debug enabled\n");
#endif
}

void print_candidates(board_t *board)
{
	int i, j;
	uint8_t num;

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			printf("(%d,%d) (%d) : ", i, j, count_cands(board, i, j));
			for (num = 1; num <= WIDTH; ++num) {
				if (is_cand(board, num, i, j)) {
					printf("%" PRIu8 " ", num);
				}
			}
			printf("\n");
		}
	}
}

void test_board(void)
{
	int i,j;
	uint8_t num;
	board_t board;
	
	/* set_easy_board(&board); */
	set_hard_board(&board);

	print_board(&board);
	assert(consistent(&board));
	assert(!solved(&board));
	board_init(&board);
	assert(is_cand(&board, 1, 3, 4));
	knockout_cand(&board, 1, 3, 4);
	assert(!is_cand(&board, 1, 3, 4));
	assert(is_cand(&board, 2, 3, 4));
	allow_cand(&board, 1, 3, 4);
	assert(is_cand(&board, 2, 3, 4));
	
	printf("Row 2 stats:\n");
	for (num = 1; num <= WIDTH; ++num) {
		printf("%" PRIu8 ": count = %d\n", 
				num, count_num_row(&board, num, 2));
	}

	printf("Col 1 stats:\n");
	for (num = 1; num <= WIDTH; ++num) {
		printf("%" PRIu8 ": count = %d\n", 
				num, count_num_col(&board, num, 1));
	}
	
	printf("Box 8 stats:\n");
	for (num = 1; num <= WIDTH; ++num) {
		printf("%" PRIu8 ": count = %d\n", 
				num, count_num_box(&board, num, 8));
	}

	printf("Candidates:\n");
	calculate_cand(&board);
	print_candidates(&board);

	printf("Solving up to first branch...\n");
	solve_upto_branch(&board);
	print_board(&board);

	assert(consistent(&board));

	if (solved(&board)) {
		printf("solved\n");
		assert(min_cand_rc(&board, &i, &j) == -1);
	} else {
		printf("need to branch\n");
	}
}

void test_solve(void)
{
	board_t board;
	
	set_hard_board2(&board);
	printf("input board:\n");
	print_board(&board);
	assert(consistent(&board));
	
	printf("solving...\n");
	solve(&board, true);
	assert(consistent(&board));

	printf("solution:\n");
	print_board(&board);
	print_candidates(&board);
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
		exit(0);
	}

	if (tflag) {
		printf("Testing...\n");
		printf("Board size: %lu bytes\n", sizeof(board_t));
		/*test_board();*/
		test_solve();
	}

	return 0;
}
