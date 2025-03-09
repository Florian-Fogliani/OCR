#ifndef _PRET_TOCUT_H
#define _PRET_TOCUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <string.h>

void rT2(SDL_Renderer *r, SDL_Texture *t, double a, SDL_RendererFlip f);

void draw2(SDL_Renderer *renderer, SDL_Texture *texture);

void rotation2(SDL_Renderer *r, SDL_Texture *t, double a, char *n);

SDL_Surface *load_image2(const char *path);

Uint32 pixel_to_grayscale2(Uint32 pixel_color, SDL_PixelFormat *format);

SDL_Surface *surface_to_grayscale2(SDL_Surface *surface);

void threshold_image2(SDL_Surface *surface, size_t val);

void tocut(int angle, char *img);
#endif
