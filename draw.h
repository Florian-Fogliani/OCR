#ifndef _DRAW_H
#define _DRAW_H

#include <SDL2/SDL.h>
#include <err.h>

// draw numbers
void draw_one(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_two(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_tree(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_four(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_five(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_six(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_seven(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_eight(SDL_Renderer *renderer, int w, int h, int x, int y);
void draw_nine(SDL_Renderer *renderer, int w, int h, int x, int y);

// draw grid
void draw_grid(SDL_Renderer *renderer, int w, int h);
void draw(SDL_Renderer *renderer, int w, int h);
void event_loop(SDL_Renderer *renderer);

int main(int argc, char **argv);

#endif
