#include "sudokulib.h"
#include <assert.h>
#include <stdio.h> /* printf */

/* debug features */
#define IMPLIES(a, b) ((!(a)) || (b))

bool valid_index(int i, int j) {
	return (0 <= i && i < N && 0 <= j && j < N);
}

bool valid_val(int val) {
	return (1 <= val && val <= N);
}

bool valid_board(const state_t *board) 
{
	int i, j, val, nallowed;

	/* check null pointer */
	if (!board) {
		return false;
	}
	
	/* check consistency of alloweds */
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			val = board->val[i][j];
			nallowed = num_allowed(board, i, j);
			
			if (!(0 <= val && val <= 9)) 
				return false;
			if (!IMPLIES(val == 0, 1 <= nallowed && nallowed <= 9)) 
				return false;
		}
	}
	return true;
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

state_t sample_state(int puzzle_no) {
	int i, j;
	state_t board;
	uint8_t puzzles[][N][N] = {
		/* websudoku level 1 Easy Puzzle 7,254,987,681 */
		{
			{0, 9, 0,  7, 3, 1,  0, 0, 0},
			{0, 6, 0,  0, 0, 0,  1, 0, 0},
			{1, 0, 3,  6, 5, 2,  0, 0, 0},
			{9, 0, 1,  3, 6, 7,  0, 0, 4},
			{3, 0, 0,  0, 0, 0,  0, 0, 5},
			{6, 0, 0,  1, 4, 5,  3, 0, 9},
			{0, 0, 0,  4, 7, 3,  8, 0, 6},
			{0, 0, 2,  0, 0, 0,  0, 3, 0},
			{0, 0, 0,  5, 2, 8,  0, 4, 0}
		},
		/* websudoku level 3 Hard Puzzle 7,865,698,460 */
		{
			{0, 0, 0,  0, 0, 5,  0, 3, 4},
			{0, 1, 5,  0, 2, 0,  9, 0, 0},
			{3, 0, 0,  0, 0, 0,  0, 7, 0},
			{0, 0, 0,  0, 5, 3,  0, 0, 0},
			{4, 6, 2,  0, 0, 0,  3, 1, 5},
			{0, 0, 0,  2, 4, 0,  0, 0, 0},
			{0, 5, 0,  0, 0, 0,  0, 0, 8},
			{0, 0, 3,  0, 9, 0,  7, 5, 0},
			{9, 4, 0,  5, 0, 0,  0, 0, 0}
		},
		/* contradictory puzzle */
		{
			{0, 0, 0,  0, 0, 5,  0, 3, 4},
			{0, 1, 5,  0, 2, 0,  9, 0, 0},
			{3, 0, 0,  0, 0, 0,  1, 7, 0},
			{0, 0, 0,  0, 5, 3,  0, 0, 0},
			{4, 6, 2,  0, 0, 0,  3, 1, 5},
			{0, 0, 0,  2, 4, 0,  0, 0, 0},
			{0, 5, 0,  0, 0, 0,  0, 0, 8},
			{0, 0, 3,  0, 9, 0,  7, 5, 0},
			{9, 4, 0,  5, 0, 0,  0, 0, 0}
		}
	};
	int npuzzles = sizeof(puzzles)/sizeof(puzzles[0]);
	assert( 0 <= puzzle_no && puzzle_no < npuzzles );
	
	board = blank_state();
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (puzzles[puzzle_no][i][j]) {
				set_given_disallow_others(&board, i, j, puzzles[puzzle_no][i][j]);
			}
		}
	}
	
	return board;
}

/* solving */
int num_allowed(const state_t *board, int i, int j)
{
	int num = 0;
	int val;
	assert( valid_index(i, j) );

	for (val = 1; val <= N; ++val) {
		if (is_allowed(board, i, j, val)) {
			++num;
		}
	}

	assert (0 <= num && num <= 9);
	return num;
}

int first_allowed(const state_t *board, int i, int j)
{
	int val;
	assert( valid_index(i, j) );
	
	for (val = 1; val <= N; ++val) {
		if (is_allowed(board, i, j, val)) {
			return val;
		}
	}
	return 0; /* none allowed */
}

void knockout(state_t *board, const int i, const int j, const int val)
{
	int r, c;
	int lo_r, lo_c;

	assert( valid_index(i, j) );
	assert( valid_val(val) );
	/* assert( valid_board(board) ); */

	/* knockout row */
	for (c = 0; c < N; ++c) {
		if (c == j) 
			continue;
		disallow(board, i, c, val);
	}

	/* knockout column */
	for (r = 0; r < N; ++r) {
		if (r == i) 
			continue;
		disallow(board, r, j, val);
	}

	/* knockout block */
	lo_r = M*(i / M);  assert(lo_r == 0 || lo_r == 3 || lo_r == 6);
	lo_c = M*(j / M);  assert(lo_c == 0 || lo_c == 3 || lo_c == 6);
	for (r = lo_r; r < lo_r + M; ++r) {
		for (c = lo_c; c < lo_c + M; ++c) {
			if (r == i && c == j)
				continue;
			disallow(board, r, c, val);
		}
	}
}

int simplify(state_t *board)
{
	int i, j;
	int npasses = 0;
	bool simplify_again = true;
	int nallowed = 0;

	assert( valid_board(board) );

	while (simplify_again) {
		/* simplify pass */
		npasses += 1;
		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				if (board->val[i][j] != 0) {
					/* given found */
					knockout(board, i, j, board->val[i][j]);
					set_given_disallow_others(board, i, j, board->val[i][j]);
				}
			}
		}

		/* check pass */
		simplify_again = false;
		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				nallowed = num_allowed(board, i, j);
				if (nallowed == 0) {
					return -1; /* contradiction reached */
				}
				if (board->val[i][j] == 0 && nallowed == 1) {
					/* only one possible value */
					board->val[i][j] = first_allowed(board, i, j);
					assert( valid_val(board->val[i][j]) );
					simplify_again = true;
				}
			}
		}
	}
	return npasses;
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
