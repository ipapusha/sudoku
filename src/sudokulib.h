#ifndef _SUDOKULIB_H
#define _SUDOKULIB_H

#include <stdbool.h>

/* data types for storing current board information */
typedef char ** board_t;
#define WIDTH 9 
#define BOXWIDTH 3

/* validity functions */
bool valid_idx(int idx);	/* row, col, or box */
bool valid_num(char num);	/* number to put in square */

/* utility functions */
int box_row(int box);
int box_col(int box);
int count_num_row(board_t board, char num, int row);
int count_num_col(board_t board, char num, int col);
int count_num_box(board_t board, char num, int box);
bool check_consistency(board_t board);
/* TODO: check_consistency_fast, 
 * check_solved, check_solved_fast */

/* IO functions */
void set_easy_board(board_t board);
void set_hard_board(board_t board);

/* solving functions */


#endif
