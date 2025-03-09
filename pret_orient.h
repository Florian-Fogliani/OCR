#ifndef PRET_ORIENT_H
#define PRET_ORIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <string.h>

void rT3(SDL_Renderer *r, SDL_Texture *t, double a, SDL_RendererFlip f);

void draw3(SDL_Renderer *renderer, SDL_Texture *texture);

void rotation3(SDL_Renderer *r, SDL_Texture *t, double a, char *n);

SDL_Surface *load_image3(const char *path);

Uint32 pixel_to_grayscale3(Uint32 pixel_color, SDL_PixelFormat *format);

SDL_Surface *surface_to_grayscale3(SDL_Surface *surface);

void orient(int angle, char *img);

#endif
