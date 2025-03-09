#ifndef HOUGH
#define HOUGH

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

double Calculate_Diagonal(SDL_Surface *img);
int *Init_Mat(const int R);
int Fill_Mat(SDL_Surface *img, int *mat, const int diag_size);
void Debug(int *mat, const int diag_size, char *img, int w, int h, int max);
void drawLine(SDL_Renderer *renderer, int rho, int theta, int width,
              int height);
struct Point
{
    int x;
    int y;
};
struct Line
{
    int rho;
    int theta;
};
void Debug_GetLines(struct Line *horizontals, struct Line *verticals,
                    int *size_horizontals, int *size_verticals, int w, int h,
                    char *img, int type);
void GetLines(int *mat, const int diag_size, int max, struct Line **horizontals,
              struct Line **verticals, int *size_horizontals,
              int *size_verticals, SDL_Surface *img);
void Insert_Sort(struct Line **tab, int *len, struct Line *line);
struct Point GetIntersec(int p1, int o1, int p2, int o2);
void SaveCas(SDL_Surface *img, int nb_l, int nb_col, struct Point bord_up,
             struct Point bord_down);
void SaveGrid(SDL_Surface *img, int nb_l, int nb_col, struct Point bord_up,
              struct Point bord_down);
void Cut(struct Line **horizontals, struct Line **verticals, int *size_h,
         int *size_v, SDL_Surface *img);
struct Line *get_10_lines(struct Line *tab, int len, int threshold);
void invertColors(SDL_Surface *surface);
struct Line *get_10_refs(struct Line *tab, int len, int threshold, int ref,
                         int i);
void CutFinale(SDL_Surface *img);
void Free_Lines(struct Line *h, struct Line *v);
void Free_Mat(int *mat);
#endif
