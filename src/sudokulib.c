#include "sudokulib.h"
#include <assert.h>

bool valid_idx(int idx) 
{
	return (0 <= idx) && (idx < WIDTH);
}

bool valid_num(char num)
{
	return (1 <= num) && (num <= WIDTH);
}

int box_row(int box) 
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

int box_col(int box)
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

int count_num_row(board_t board, char num, int row)
{
	int j;
	int count = 0;

	assert(valid_idx(row));

	for (j = 0; j < WIDTH; ++j) {
		if (board[row][j] == num) {
			++count;
		}
	}
	
	return count;
}

int count_num_col(board_t board, char num, int col)
{
	int i;
	int count = 0;

	assert(valid_idx(col));

	for (i = 0; i < WIDTH; ++i) {
		if (board[i][col] == num) {
			++count;
		}
	}
	
	return count;
}

int count_num_box(board_t board, char num, int box)
{
	int i, j;
	int i0, j0; 
	int count = 0;

	assert(valid_idx(box));

	i0 = box_row(box);
	j0 = box_col(box);

	for (i = i0; i < i0 + BOXWIDTH; ++i) {
		for (j = j0; j < j0 + BOXWIDTH; ++j) {
			if (board[i][j] == num) {
				++count;
			}
		}
	}
	
	return count;
}

bool check_consistency(board_t board)
{
	int i,j,k;
	char num;
	for (num = 1; num <= WIDTH; ++num) {
		/* check each row */
		for (i = 0; i < WIDTH; ++i) {
			if (count_num_row(board, num, i) > 1) {
				return false;
			}
		}

		/* check each col */
		for (j = 0; j < WIDTH; ++j) {
			if (count_num_col(board, num, j) > 1) {
				return false;
			}
		}

		/* check each box */
		for (k = 0; k < WIDTH; ++k) {
			if (count_num_box(board, num, k) > 1) {
				return false;
			}
		}
	}
	return true;
}

void set_easy_board(board_t board) 
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
			board[i][j] = (char)arr[i][j];
		}
	}
}

void set_hard_board(board_t board) 
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
			board[i][j] = (char)arr[i][j];
		}
	}
}

