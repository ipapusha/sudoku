#ifndef _SUDOKULIB_H
#define _SUDOKULIB_H

#include <stdint.h>
#include <stdbool.h>

#define N 9
#define M (N/3)

typedef struct state_t {
	uint8_t val[N][N]; 
	uint_least16_t allowed[N][N];
} state_t;

/* state manipulation */
void set_allowed(state_t *board, int i, int j, int val, bool allow_flag);
bool is_allowed(const state_t *board, int i, int j, int val);
void allow(state_t *board, int i, int j, int val);
void disallow(state_t *board, int i, int j, int val);

void set_given(state_t *board, int i, int j, int val);
void set_given_disallow_others(state_t *board, int i, int j, int val);

/* initialization */
state_t blank_state(void);
state_t sample_state(int puzzle_no);

/* solving */
int num_allowed(const state_t *board, int i, int j);
int first_allowed(const state_t *board, int i, int j);
void knockout(state_t *board, int i, int j, int val); /* modifies all but i,j */
int simplify(state_t *board);
int solve(state_t *board);
bool is_solved(const state_t *board);

/* IO */
void print_state(const state_t *board);

#endif
