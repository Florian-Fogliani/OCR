#include "solver.h"

struct position *new_elt(int nb, int pos)
// create a new block not linked to the list chainer returns its location
{
    struct position *res = malloc(sizeof(struct position));
    if (res != NULL)
    {
        res->nb_v = nb;
        res->pos = pos;
        res->next = NULL;
    }
    return res;
}

void free_l(struct position *p) // free the linked liste
{
    struct position *tmp = p;
    while (tmp != NULL)
    {
        p = tmp->next;
        free(tmp);
        tmp = p;
    }
}

void insert(struct position **p, struct position *new)
// stores a new element according to the number of possible values
// in a sorted chain list
{
    if (*p == NULL) // liste parcouru en entier/ placement à la fin
    {
        *p = new;
        new->next = NULL;
    }
    else if ((*p)->nb_v < new->nb_v) // recherche de l'emplacement
    {
        insert(&(*p)->next, new);
    }
    else // emplacent trouver
    {
        new->next = *p;
        *p = new;
    }
}

////////////////////////// chose the number  /////////////////////////////

bool col_v(int n, int *grid, int col) // test if the number can be in the column
{
    int i = 0;
    while (i < 9)
    {
        if (grid[i * 9 + col] == n)
            return false;
        i++;
    }
    return true;
}

bool row_v(int n, int *grid, int row) // test if the number can be in the row
{
    int j = 0;
    while (j < 9)
    {
        if (grid[row * 9 + j] == n)
            return false;
        j++;
    }
    return true;
}

bool bloc_v(int n, int *grid, int col, int row)
// test if the number can be in the bloc
{
    int i = row - (row % 3); // prevents from leaving the grid
    int j = col - (col % 3);
    int _i = i;
    int _j = j;
    while (i < _i + 3)
    {
        j = _j;
        while (j < _j + 3)
        {
            if (grid[i * 9 + j] == n)
                return false;
            j++;
        }
        i++;
    }
    return true;
}

bool nb_v(int n, int *grid, int pos) // test if the number can be in the box
{
    int row = pos / 9;
    int col = pos % 9;
    return col_v(n, grid, col) && row_v(n, grid, row)
        && bloc_v(n, grid, col, row);
}

int nb_pos(int *grid, int pos)
{
    int n = 0;
    for (int i = 1; i <= 9; i++)
    {
        if (nb_v(i, grid, pos))
            n += 1;
    }
    return n;
}

////////////////////////////////////////////////////////resolution//////////

bool solve(int *grid, struct position *Pos) // solve the grid
{
    if (Pos == NULL)
        return true;
    int pos = Pos->pos;
    // backtracking
    int n = 1;
    while (n < 10)
    {
        if (nb_v(n, grid, pos))
        {
            grid[pos] = n;
            if (solve(grid, Pos->next))
                return true;
        }
        n += 1;
    }
    grid[pos] = 0;
    return false;
}

bool main_solve(int *grid) // calls the solver with the linked list
{
    struct position *Pos = NULL;
    int i = 0;
    int j = 0;
    while (i < 9)
    {
        j = 0;
        while (j < 9)
        {
            if (grid[i * 9 + j] == 0)
            {
                int nb = nb_pos(grid, i * 9 + j);
                struct position *tmp = new_elt(nb, i * 9 + j);
                insert(&Pos, tmp);
            }
            j++;
        }
        i++;
    }
    bool r = solve(grid, Pos);
    free_l(Pos);
    return r;
}

////////////////////////formatting////////////////////////////////////
int **read_f(char *path)
{
    int **r = malloc(2 * sizeof(int *));
    int *deb = malloc(81 * sizeof(int));
    int *grid = malloc(81 * sizeof(int));
    r[0] = grid;
    r[1] = deb;
    FILE *file = NULL;
    int c = 0;
    size_t i = 0;
    file = fopen(path, "r");
    if (file == NULL)
    {
        err(1, "unfind file");
    }
    else
    {
        do
        {
            c = fgetc(file);
            if (c != ' ' && c != '\n')
            {
                if (i > 81)
                    err(1, "wrong file");
                else if (c == '.')
                {
                    grid[i] = 0;
                    deb[i] = 0;
                }
                else
                {
                    grid[i] = c - '0';
                    deb[i] = 1;
                }
                i++;
            }
        } while (c != EOF);

        fclose(file);
        return r;
    }
}
void write_f(char *path, int *grid)
{
    FILE *file = NULL;
    size_t t = strlen(path);
    path = realloc(path, (t + 7) * sizeof(char));
    path[t] = '.';
    path[t + 1] = 'r';
    path[t + 2] = 'e';
    path[t + 3] = 's';
    path[t + 4] = 'u';
    path[t + 5] = 'l';
    path[t + 6] = 't';
    path[t + 7] = 0;
    file = fopen(path, "w");
    free(path);
    if (file == NULL)
    {
        err(1, "unfind file");
    }
    else
    {
        for (int i = 0; i < 9 * 9; i++)
        {
            if (i % 27 == 0 && i != 0)
            {
                fputc('\n', file);
                fputc('\n', file);
            }
            else if (i % 9 == 0 && i != 0)
                fputc('\n', file);
            else if (i % 3 == 0 && i != 0)
                fputc(' ', file);

            fputc('0' + grid[i], file);
        }
        fputc('\n', file);
        fclose(file);
    }
}

///////////////////////////////Main////////////////////////////////////////

int **do_all_solve(int argc, char *argv[])
{
    // double time_spent = 0.0;
    // clock_t begin = clock();

    if (argc != 2)
        err(1, "wrong parameter");
    char *path = strdup(argv[1]);
    int **grid = read_f(path);
    main_solve(grid[0]);
    write_f(path, grid[0]);
    return grid;
    // clock_t end = clock();
    // time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("The elapsed time is %f seconds\n", time_spent);
}
