#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit */
#include <getopt.h> /* getopt, GNUed */
#include "sudokulib.h"

void print_versioninfo(void)
{
	printf("sudoku v0.2\n");
	printf("Author: Ivan Papusha\n");
	printf("Board size: %lu bytes\n", sizeof(board_t));
#ifdef NDEBUG
	printf("debug disabled\n");
#else
	printf("debug enabled\n");
#endif
}

int main(int argc, char **argv)
{
	int vflag = 0;
	int c;

	while ((c = getopt(argc, argv, "vt")) != -1) {
		switch (c) {
			case 'v': 
				vflag = 1; 
				break;
			default:
				break;
		}
	}

	if (vflag) {
		print_versioninfo();
		exit(0);
	}

	return 0;
}
