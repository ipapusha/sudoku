#ifndef _SUDOKULIB_H
#define _SUDOKULIB_H

#include <stdint.h>
#include <stdbool.h>

/* data types for storing current board information
 *  indices 0..8 are called i,j or row,col
 *  boxes 0..8 are called k or box
 *  when irrelevant, indices or boxes are called idx
 *  values 1..9 are called num
 */
#define WIDTH 9 
#define BOXWIDTH 3
typedef struct board_t {
	/* pos[i][j] in {1,...,9} is a number assignment */
	uint8_t pos[WIDTH][WIDTH]; 

	/* for each num = 1..9, the num-th bit of cand[i][j], i.e.,
	 * cand[i][j] & (1 << num), is zero if and only if num is a
	 * candidate number allowed to be placed at position i,j. 
	 */
	uint_least16_t cand[WIDTH][WIDTH];
} board_t;

/* validity functions */
bool valid_idx(int idx);
bool valid_num(uint8_t num);

/* utility functions */
int box2row(int box);
int box2col(int box);
int rc2box(int row, int col);
int count_num_row(const board_t *board, uint8_t num, int row);
int count_num_col(const board_t *board, uint8_t num, int col);
int count_num_box(const board_t *board, uint8_t num, int box);
bool is_cand(const board_t *board, uint8_t num, int row, int col);
void allow_cand(board_t *board, uint8_t num, int row, int col);
void clear_cand(board_t *board, uint8_t num, int row, int col);
void clear_all_cands_at(board_t *board, int row, int col);
void board_init(board_t *board);
void knockout_cand(board_t *board, uint8_t num, int row, int col);
void knockout_cand_row(board_t *board, uint8_t num, int row);
void knockout_cand_col(board_t *board, uint8_t num, int col);
void knockout_cand_box(board_t *board, uint8_t num, int box);

/* solving functions */
bool consistent(const board_t *board);	/* ignores cand */
bool solved(const board_t *board);		/* ignores cand */
void calculate_cand(board_t *board);
/* TODO: consistent_fast, solved_fast */

/* IO functions */
void set_easy_board(board_t *board);
void set_hard_board(board_t *board);
void print_board(const board_t *board);


#endif
