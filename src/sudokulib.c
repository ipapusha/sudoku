#include "sudokulib.h"
#include <assert.h>
#include <stdio.h> /* printf */

/* debug features */
bool valid_index(int i, int j) {
	return (0 <= i && i < N && 0 <= j && j < N);
}

bool valid_val(int val) {
	return (1 <= val && val <= N);
}

/* implementations */
void set_allowed(state_t *board, int i, int j, int val, bool allow_flag) {
	assert( valid_index(i, j) );
	assert( valid_val(val) );

	if (allow_flag) {
		board->allowed[i][j] |= ((unsigned)1 << val);
	} else {
		board->allowed[i][j] &= ~((unsigned)1 << val);
	}
}

bool is_allowed(const state_t *board, int i, int j, int val) {
	assert( valid_index(i, j) );
	assert( valid_val(val) );
	
	return (board->allowed[i][j] & ((unsigned)1 << val));
}

void allow(state_t *board, int i, int j, int val) {
	set_allowed(board, i, j, val, true);
}

void disallow(state_t *board, int i, int j, int val) {
	set_allowed(board, i, j, val, false);
}

void set_given(state_t *board, int i, int j, int val)
{
	assert( valid_index(i, j) );
	assert( valid_val(val) );

	board->val[i][j] = val;
}

void set_given_disallow_others(state_t *board, int i, int j, int val)
{
	int other_val;
	set_given(board, i, j, val);
	
	for (other_val = 1; other_val <= N; ++other_val) {
		if (val == other_val) {
			allow(board, i, j, other_val);
		} else {
			disallow(board, i, j, other_val);
		}
	}
}

/* initialization */
state_t blank_state(void) {
	state_t board;
	int i, j, val;

	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			board.val[i][j] = 0;
			for (val = 1; val <= N; ++val) {
				allow(&board, i, j, val);
			}
		}
	}
	return board;
}

state_t easy_state(void) {
	int i, j;
	state_t board;
	/* websudoku level 1 Easy Puzzle 7,254,987,681 */
	uint8_t vals[N][N] = {
		{0, 9, 0,  7, 3, 1,  0, 0, 0},
		{0, 6, 0,  0, 0, 0,  1, 0, 0},
		{1, 0, 3,  6, 5, 2,  0, 0, 0},
		{9, 0, 1,  3, 6, 7,  0, 0, 4},
		{3, 0, 0,  0, 0, 0,  0, 0, 5},
		{6, 0, 0,  1, 4, 5,  3, 0, 9},
		{0, 0, 0,  4, 7, 3,  8, 0, 6},
		{0, 0, 2,  0, 0, 0,  0, 3, 0},
		{0, 0, 0,  5, 2, 8,  0, 4, 0}
	};

	board = blank_state();
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (vals[i][j]) {
				set_given_disallow_others(&board, i, j, vals[i][j]);
			}
		}
	}
	
	return board;
}

state_t hard_state(void) {
	int i, j;
	state_t board;
	/* websudoku level 3 Hard Puzzle 7,865,698,460 */
	uint8_t vals[N][N] = {
		{0, 0, 0,  0, 0, 5,  0, 3, 4},
		{0, 1, 5,  0, 2, 0,  9, 0, 0},
		{3, 0, 0,  0, 0, 0,  0, 7, 0},
		{0, 0, 0,  0, 5, 3,  0, 0, 0},
		{4, 6, 2,  0, 0, 0,  3, 1, 5},
		{0, 0, 0,  2, 4, 0,  0, 0, 0},
		{0, 5, 0,  0, 0, 0,  0, 0, 8},
		{0, 0, 3,  0, 9, 0,  7, 5, 0},
		{9, 4, 0,  5, 0, 0,  0, 0, 0}
	};

	board = blank_state();
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (vals[i][j]) {
				set_given_disallow_others(&board, i, j, vals[i][j]);
			}
		}
	}
	
	return board;
}

/* IO */
void print_state(const state_t *board)
{
	int i, j;

	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			printf("%d ", board->val[i][j]);
		}
		printf("\n");
	}
}
