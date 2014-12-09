#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit */
#include <getopt.h> /* getopt, GNUed */
#include "sudokulib.h"

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
	state_t board0, board1;

	board0 = sample_state(0);
	board1 = sample_state(1);
	print_state(&board0);
	printf("\n");
	print_state(&board1);
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
