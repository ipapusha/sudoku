#include "sudokulib.h"
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <inttypes.h>
#include <assert.h>

bool valid_idx(int idx) 
{
	return (0 <= idx) && (idx < WIDTH);
}

bool valid_num(uint8_t num)
{
	return (1 <= num) && (num <= WIDTH);
}

int box2row(int box) 
{
	assert(valid_idx(box));
	switch (box) {
		case 0:
		case 1:
		case 2:
			return 0;
			break;
		case 3:
		case 4:
		case 5:
			return 3;
			break;
		case 6:
		case 7:
		case 8:
			return 6;
			break;
		default:
			assert(0);
	}
	return -1;
}

int box2col(int box)
{
	assert(valid_idx(box));
	switch (box) {
		case 0:
		case 3:
		case 6:
			return 0;
			break;
		case 1:
		case 4:
		case 7:
			return 3;
			break;
		case 2:
		case 5:
		case 8:
			return 6;
			break;
		default:
			assert(0);
	}
	return -1;
}

int rc2box(int row, int col)
{
	int box;

	assert(valid_idx(row));
	assert(valid_idx(col));

	box = (row / 3) * 3 + (col / 3);
	
	assert(valid_idx(box));
	assert(box2row(box) <= row && row < box2row(box) + BOXWIDTH);
	assert(box2col(box) <= col && col < box2col(box) + BOXWIDTH);

	return box;
}

int count_num_row(const board_t *board, uint8_t num, int row)
{
	int j;
	int count = 0;

	assert(valid_idx(row));

	for (j = 0; j < WIDTH; ++j) {
		if (board->pos[row][j] == num) {
			++count;
		}
	}
	
	return count;
}

int count_num_col(const board_t *board, uint8_t num, int col)
{
	int i;
	int count = 0;

	assert(valid_idx(col));

	for (i = 0; i < WIDTH; ++i) {
		if (board->pos[i][col] == num) {
			++count;
		}
	}
	
	return count;
}

int count_num_box(const board_t *board, uint8_t num, int box)
{
	int i,j;
	int row, col;
	int count = 0;

	assert(valid_idx(box));
	row = box2row(box);
	col = box2col(box);

	for (i = row; i < row + BOXWIDTH; ++i) {
		for (j = col; j < col + BOXWIDTH; ++j) {
			if (board->pos[i][j] == num) {
				++count;
			}
		}
	}
	
	return count;
}

bool is_cand(const board_t *board, uint8_t num, int row, int col)
{
	assert(valid_num(num));
	assert(valid_idx(row));
	assert(valid_idx(col));
	
	return !(board->cand[row][col] & ((uint_least16_t)1 << num));
}

int count_cands(const board_t *board, int row, int col)
{
	int num;
	int count = 0;

	assert(valid_idx(row));
	assert(valid_idx(col));

	for (num = 1; num <= WIDTH; ++num) {
		if (is_cand(board, num, row, col)) {
			++count;
		}
	}

	return count;
}

void allow_cand(board_t *board, uint8_t num, int row, int col)
{
	assert(valid_num(num));
	assert(valid_idx(row));
	assert(valid_idx(col));
	
	board->cand[row][col] &= ~((uint_least16_t)1 << num);
}

void clear_cand(board_t *board, uint8_t num, int row, int col)
{
	assert(valid_num(num));
	assert(valid_idx(row));
	assert(valid_idx(col));

	board->cand[row][col] |= ((uint_least16_t)1 << num);
}

void clear_all_cands_at(board_t *board, int row, int col)
{
	uint8_t num;
	for (num = 1; num <= WIDTH; ++num) {
		clear_cand(board, num, row, col);
	}
}

void board_init(board_t *board) 
{
	int i,j;
	uint8_t num;

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			for (num = 1; num <= WIDTH; ++num) {
				allow_cand(board, num, i, j);
				assert(is_cand(board, num, i, j));
			}
		}
	}
}

void knockout_cand(board_t *board, uint8_t num, int row, int col)
{
	assert(valid_num(num));
	assert(valid_idx(row));
	assert(valid_idx(col));

	board->cand[row][col] |= (1 << num);
}

void knockout_cand_row(board_t *board, uint8_t num, int row)
{
	int j;
	for (j = 0; j < WIDTH; ++j) {
		knockout_cand(board, num, row, j);
	}
}

void knockout_cand_col(board_t *board, uint8_t num, int col)
{
	int i;
	for (i = 0; i < WIDTH; ++i) {
		knockout_cand(board, num, i, col);
	}
}

void knockout_cand_box(board_t *board, uint8_t num, int box)
{
	int i,j;
	int row, col;

	row = box2row(box);
	col = box2col(box);
	for (i = row; i < row + BOXWIDTH; ++i) {
		for (j = col; j < col + BOXWIDTH; ++j) {
			knockout_cand(board, num, i, j);
		}
	}
}

bool consistent(const board_t *board)
{
	int i, j, k;
	uint8_t num;
	
	/* check each row */
	for (i = 0; i < WIDTH; ++i) {
		for (num = 1; num <= WIDTH; ++num) {
			if (count_num_row(board, num, i) > 1) {
				return false;
			}
		}
	}
	
	/* check each col */
	for (j = 0; j < WIDTH; ++j) {
		for (num = 1; num <= WIDTH; ++num) {
			if (count_num_col(board, num, j) > 1) {
				return false;
			}
		}
	}

	/* check each box */
	for (k = 0; k < WIDTH; ++k) {
		for (num = 1; num <= WIDTH; ++num) {
			if (count_num_box(board, num, k) > 1) {
				return false;
			}
		}
	}

	return true;
}

bool solved(const board_t *board)
{
	int i, j, k;
	uint8_t num;
	
	/* check each row */
	for (i = 0; i < WIDTH; ++i) {
		for (num = 1; num <= WIDTH; ++num) {
			if (count_num_row(board, num, i) != 1) {
				return false;
			}
		}
	}
	
	/* check each col */
	for (j = 0; j < WIDTH; ++j) {
		for (num = 1; num <= WIDTH; ++num) {
			if (count_num_col(board, num, j) != 1) {
				return false;
			}
		}
	}

	/* check each box */
	for (k = 0; k < WIDTH; ++k) {
		for (num = 1; num <= WIDTH; ++num) {
			if (count_num_box(board, num, k) != 1) {
				return false;
			}
		}
	}

	return true;
}

void calculate_cand(board_t *board)
{
	int i, j;
	uint8_t num;

	assert(consistent(board));

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			num = board->pos[i][j];
			if (num) {
				clear_all_cands_at(board, i, j);
				knockout_cand_row(board, num, i);
				knockout_cand_col(board, num, j);
				knockout_cand_box(board, num, rc2box(i,j));
			}
		}
	}

	assert(consistent(board));
}


/* set (*row) and (*col) to an index with the smallest 
 * nonzero number of candidates and returns the number 
 * of candidates at that index (or -1 if not defined)
 */
int min_cand_rc(const board_t *board, int *row, int *col)
{
	int i,j;
	int count;
	int min_count = WIDTH + 1;

	assert(consistent(board));

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			count = count_cands(board, i, j);
			if (count && (count < min_count)) {
				min_count = count;
				*row = i;
				*col = j;
				/* short-circuit */
				if (count == 1) {
					return count;
				}
			}
		}
	}
	
	if (min_count == WIDTH + 1) {
		*row = -1;
		*col = -1;
		return -1;
	} 
	return min_count;
}

void solve_upto_branch(board_t *board)
{
	int i,j;
	uint8_t num;

	/* initial solve */
	calculate_cand(board);

	/* continue solving if possible */
	while(min_cand_rc(board, &i, &j) == 1) {
		/* determine single candidate remaining */
		for (num = 1; num <= WIDTH; ++num) {
			if (is_cand(board, num, i, j)) {
				break;
			}
		}
		assert(num <= WIDTH);
		
		/* square is determined */
		clear_all_cands_at(board, i, j);
		knockout_cand_row(board, num, i);
		knockout_cand_col(board, num, j);
		knockout_cand_box(board, num, rc2box(i,j));
		
		/* set square */
		board->pos[i][j] = num;
	}
}

void solve(board_t *board, bool initial_solve)
{
	int i,j;
	uint8_t num;
	int count;
	board_t board_saved;

	/* only run initial solve once */
	if (initial_solve) {
		calculate_cand(board);
	}
	
	/* main solving loop */
	while((count = min_cand_rc(board, &i, &j)) != -1) {
		/* determine first candidate remaining */
		for (num = 1; num <= WIDTH; ++num) {
			if (is_cand(board, num, i, j)) {
				break;
			}
		}
		assert(num <= WIDTH);
		
		if (count == 1) {
			/* square is determined */
			
			/* knock out neighbors */
			clear_all_cands_at(board, i, j);
			knockout_cand_row(board, num, i);
			knockout_cand_col(board, num, j);
			knockout_cand_box(board, num, rc2box(i,j));

			/* set square */
			board->pos[i][j] = num;
		} else {
			/* several candidates exist */
			memcpy(&board_saved, board, sizeof(board_t));
			assert(memcmp(&board_saved, board, sizeof(board_t)) == 0);
			
			/* knock out neighbors */
			clear_all_cands_at(board, i, j);
			knockout_cand_row(board, num, i);
			knockout_cand_col(board, num, j);
			knockout_cand_box(board, num, rc2box(i,j));

			/* set square */
			board->pos[i][j] = num;

			/* recurse */
			solve(board, false);
			
			if (solved(board)) {
				/* solved */
				return;
			} else {
				/* not solved, restore state */
				memcpy(board, &board_saved, sizeof(board_t));
				assert(memcmp(&board_saved, board, sizeof(board_t)) == 0);

				/* eliminate candidate from current square */
				clear_cand(board, num, i, j);
			}
		}
	}
}

void set_easy_board(board_t *board) 
{
	int i,j;
	int arr[WIDTH][WIDTH] = {
		{0, 0, 0, 8, 0, 0, 3, 9, 0},
		{0, 3, 0, 7, 0, 0, 0, 1, 5},
		{0, 0, 7, 5, 3, 6, 0, 2, 0},
		{0, 0, 5, 0, 0, 8, 0, 3, 0},
		{1, 8, 0, 0, 0, 0, 0, 4, 9},
		{0, 7, 0, 4, 0, 0, 8, 0, 0},
		{0, 9, 0, 6, 7, 1, 2, 0, 0},
		{7, 2, 0, 0, 0, 3, 0, 5, 0},
		{0, 4, 1, 0, 0, 5, 0, 0, 0}
	};

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			board->pos[i][j] = (uint8_t)arr[i][j];
		}
	}
}

void set_hard_board(board_t *board) 
{
	int i,j;
	int arr[WIDTH][WIDTH] = {
		{3, 0, 0, 4, 8, 0, 2, 0, 0},
		{0, 0, 0, 0, 0, 6, 0, 0, 0},
		{0, 6, 0, 0, 2, 5, 3, 0, 0},
		{0, 2, 1, 0, 0, 7, 8, 0, 0},
		{5, 0, 3, 0, 0, 0, 9, 0, 4},
		{0, 0, 6, 2, 0, 0, 5, 7, 0},
		{0, 0, 9, 6, 7, 0, 0, 5, 0},
		{0, 0, 0, 8, 0, 0, 0, 0, 0},
		{0, 0, 2, 0, 9, 4, 0, 0, 7}
	};

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			board->pos[i][j] = (uint8_t)arr[i][j];
		}
	}
}

void set_hard_board2(board_t *board) 
{
	int i,j;
	int arr[WIDTH][WIDTH] = {
		{0, 0, 0, 0, 0, 5, 0, 3, 4},
		{0, 1, 5, 0, 2, 0, 9, 0, 0},
		{3, 0, 0, 0, 0, 0, 0, 7, 0},
		{0, 0, 0, 0, 5, 3, 0, 0, 0},
		{4, 6, 2, 0, 0, 0, 3, 1, 5},
		{0, 0, 0, 2, 4, 0, 0, 0, 0},
		{0, 5, 0, 0, 0, 0, 0, 0, 8},
		{0, 0, 3, 0, 9, 0, 7, 5, 0},
		{9, 4, 0, 5, 0, 0, 0, 0, 0}
	};

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			board->pos[i][j] = (uint8_t)arr[i][j];
		}
	}
}

void print_board(const board_t *board)
{
	int i,j;
	uint8_t num;

	for (i = 0; i < WIDTH; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			num = board->pos[i][j];
			if (num) {
				printf("%" PRIu8 " ", board->pos[i][j]);
			} else {
				printf(". ");
			}
		}
		printf("\n");
	}
}
