#ifndef _SUDOKULIB_H
#define _SUDOKULIB_H

#include <stdint.h>
#include <stdbool.h>

#define N 9

typedef struct board_t {
	uint8_t pos[N][N]; 
	uint_least16_t cand[N][N];
} board_t;

#endif
