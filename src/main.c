#include <stdio.h>	/* printf */
#include <stdlib.h> /* exit, malloc */
#include "sudokulib.h"

#include <unistd.h> /* getopt */
#include <getopt.h>

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
	return;
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
		test_easyboard();
	}

	return 0;
}
