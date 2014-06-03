

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

void test_easyboard(void)
{
	int i,j;
	uint8_t num;
	board_t board;
	
	set_easy_board(&board);
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
	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			printf("(%d,%d) : ", i, j);
			for (num = 1; num <= WIDTH; ++num) {
				if (is_cand(&board, num, i, j)) {
					printf("%" PRIu8 " ", num);
				}
			}
			printf("\n");
		}
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
		exit(0);
	}

	if (tflag) {
		printf("Testing...\n");
		printf("Board size: %lu bytes\n", sizeof(board_t));
		test_easyboard();
	}

	return 0;
}
