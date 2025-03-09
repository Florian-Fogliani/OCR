#include "pret_orient.h"

void rT3(SDL_Renderer *r, SDL_Texture *t, double a, SDL_RendererFlip f)
{
    int wW, wH;
    SDL_GetRendererOutputSize(r, &wW, &wH);

    int tW, tH;
    SDL_QueryTexture(t, NULL, NULL, &tW, &tH);

    // Convert angle to radians
    double radA = a * M_PI / 180.0;

    // Calculate the new width and height of the oriented image
    double rW = fabs(tW * cos(radA)) + fabs(tH * sin(radA));

    double rH = fabs(tW * sin(radA)) + fabs(tH * cos(radA));

    double scaleX = (double)wW / rW;
    double scaleY = (double)wH / rH;

    //  Choose the smallest scale to maintain proportion
    double scale = fmin(scaleX, scaleY);

    // Calculate destination size according to scale
    double dstWidth = tW * scale;
    double dstHeight = tH * scale;

    SDL_Rect dstRect;
    dstRect.w = dstWidth;
    dstRect.h = dstHeight;
    dstRect.x = (wW - dstWidth) / 2;
    dstRect.y = (wH - dstHeight) / 2;

    SDL_RenderCopyEx(r, t, NULL, &dstRect, a, NULL, f);
}

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw3(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Rotation
void rotation3(SDL_Renderer *r, SDL_Texture *t, double a, char *n)
{
    // Rotation image
    rT3(r, t, a, SDL_FLIP_NONE);

    // Recover window size
    int wW, wH;
    SDL_GetRendererOutputSize(r, &wW, &wH);

    // Create a surface for the image
    SDL_Surface *i = SDL_CreateRGBSurface(0, wW, wH, 32, 0, 0, 0, 0);
    if (i == NULL)
        errx(EXIT_FAILURE, "SDL_CreateRGBSurface: %s", SDL_GetError());

    // Copy the contents of the rendering onto the surface
    SDL_RenderReadPixels(r, NULL, SDL_PIXELFORMAT_ARGB8888, i->pixels,
                         i->pitch);

    // Adjust the alpha channel of all pixels to 255
    Uint32 *pixels = (Uint32 *)i->pixels;
    int len = i->w * i->h;
    for (int j = 0; j < len; j++)
    {
        pixels[j] |= 0xFF000000;
    }

    //  Save the surface as a PNG image
    if (IMG_SavePNG(i, n) != 0)
        errx(EXIT_FAILURE, "IMG_SavePNG: %s", IMG_GetError());

    SDL_FreeSurface(i);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface *load_image3(const char *path)
{
    SDL_Surface *tmp = IMG_Load(path);
    if (tmp == NULL)
        errx(EXIT_FAILURE, "IMG_Load: %s", SDL_GetError());

    SDL_Surface *s = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    if (s == NULL)
        errx(EXIT_FAILURE, "SDL_ConvertSurfaceFormat: %s", SDL_GetError());

    SDL_FreeSurface(tmp);

    return s;
}

// Converts a colored pixel into grayscale.
Uint32 pixel_to_grayscale3(Uint32 pixel_color, SDL_PixelFormat *format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;

    return SDL_MapRGB(format, average, average, average);
}

// Convert the image surface to grayscale
SDL_Surface *surface_to_grayscale3(SDL_Surface *surface)
{
    Uint32 *pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat *format = surface->format;

    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale3(pixels[i], format);

    return surface;
}

void orient(int angle, char *img)
{
    /*    if (argc != 3)
            errx(EXIT_FAILURE, "Usage: image-file rotation_angle");

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            errx(EXIT_FAILURE, "SDL_Init: %s", SDL_GetError());*/

    SDL_WindowFlags wF = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    int wP = SDL_WINDOWPOS_CENTERED;

    //   int angle = atof(argv[2]);
    if (angle == 0)
    {
        angle = 360;
    }

    SDL_Window *w_r = SDL_CreateWindow("Rotated Image", wP, wP, 800, 800, wF);
    if (w_r == NULL)
        errx(EXIT_FAILURE, "SDL_CreateWindow: %s", SDL_GetError());

    SDL_Renderer *r_r = SDL_CreateRenderer(w_r, -1, SDL_RENDERER_ACCELERATED);
    if (r_r == NULL)
        errx(EXIT_FAILURE, "SDL_CreateRenderer: %s", SDL_GetError());

    SDL_Surface *surface_r = load_image3(img);

    if (surface_r == NULL)
        errx(EXIT_FAILURE, "load_image: %s", SDL_GetError());

    // Create a texture from the grayscale surface
    SDL_Texture *texture_r = SDL_CreateTextureFromSurface(r_r, surface_r);
    if (texture_r == NULL)
        errx(EXIT_FAILURE, "SDL_CreateTextureFromSurface: %s", SDL_GetError());

    rotation3(r_r, texture_r, angle, "img_oriented.png");
    SDL_DestroyTexture(texture_r);
    SDL_DestroyRenderer(r_r);
    SDL_DestroyWindow(w_r);
    SDL_Quit();
}
