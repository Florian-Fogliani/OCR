#ifndef _PRET_ANALYSE_H
#define _PRET_ANALYSE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <string.h>

void rT2(SDL_Renderer *r, SDL_Texture *t, double a, SDL_RendererFlip f);

void draw2(SDL_Renderer *renderer, SDL_Texture *texture);

SDL_Surface *load_image2(const char *path);

Uint32 pixel_to_grayscale2(Uint32 pixel_color, SDL_PixelFormat *format);

SDL_Surface *surface_to_grayscale2(SDL_Surface *surface);

void threshold_image(SDL_Surface *surface, size_t val, int ind);

void set_pixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);

Uint32 get_pixel(SDL_Surface *surface, int x, int y);

Uint8 get_pixel_component(SDL_Surface *surface, int x, int y, char component);

void detectContours(SDL_Surface *surface, SDL_Surface *surface2, int val);

void analyse(int angle, char *img);

#endif
