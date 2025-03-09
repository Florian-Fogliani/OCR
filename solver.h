#ifndef _SOLVER_H
#define _SOLVER_H

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/// linked list of position/////////////////////////////////////////////////

struct position // basic structure
{
    int pos; // the position in the sudoku grid
    int nb_v; // the number of values the box can take
    struct position *next; // the linked
};
// function
struct position *new_elt(int nb, int pos);
void free_l(struct position *p);
void insert(struct position **p, struct position *new);
bool col_v(int n, int *grid, int col);
bool row_v(int n, int *grid, int row);
bool bloc_v(int n, int *grid, int col, int row);
bool nb_v(int n, int *grid, int pos);
int nb_pos(int *grid, int pos);
bool solve(int *grid, struct position *Pos);
bool main_solve(int *grid);
int **read_f(char *path);
void write_f(char *path, int *grid);
int **do_all_solve(int argc, char *argv[]);
#endif
