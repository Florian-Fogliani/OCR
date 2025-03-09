#include "pret_analyse.h"

void rT(SDL_Renderer *r, SDL_Texture *t, double a, SDL_RendererFlip f)
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

// Rotation
void rotation(SDL_Renderer *r, SDL_Texture *t, double a, char *n)
{
    // Rotation image
    rT(r, t, a, SDL_FLIP_NONE);

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
// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface *load_image(const char *path)
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
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat *format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;

    return SDL_MapRGB(format, average, average, average);
}

// Convert the image surface to grayscale
SDL_Surface *surface_to_grayscale(SDL_Surface *surface)
{
    Uint32 *pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat *format = surface->format;

    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);

    return surface;
}

// Threshold the image based on the average pixel value
void threshold_image(SDL_Surface *surface, size_t val, int ind)
{
    Uint32 *pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat *format = surface->format;

    Uint32 totalAverage = 0;

    // Calculate the total average pixel value
    for (int i = 0; i < len; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        totalAverage += (r + g + b) / 3;
    }

    // Calculate the overall average
    Uint32 average = totalAverage / len;

    // Threshold the image based on the average pixel value
    for (int i = 0; i < len; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if (ind == 1)
        {
            if ((Uint8)((r + g + b) / 3) < (Uint8)(average - val))
            {
                pixels[i] = SDL_MapRGB(format, 255, 255, 255); // Set to white
            }
            else
            {
                pixels[i] = SDL_MapRGB(format, 0, 0, 0); // Set to black
            }
        }
        else
        {
            if ((Uint8)((r + g + b) / 3) < (Uint8)(average - val))
            {
                pixels[i] = SDL_MapRGB(format, 0, 0, 0); // Set to white
            }
            else
            {
                pixels[i] = SDL_MapRGB(format, 255, 255, 255); // Set to black
            }
        }
    }
}

// Define the colour of a pixel on an SDL surface
void set_pixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
    {
        Uint32 pixelValue = SDL_MapRGB(surface->format, r, g, b);
        Uint8 *pixelAddress = (Uint8 *)surface->pixels + y * surface->pitch
            + x * surface->format->BytesPerPixel;
        *(Uint32 *)pixelAddress = pixelValue;
    }
}

// Obtaining the colour of a pixel from an SDL surface
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    // Calculate the index of the pixel in the array of pixels in the surface
    int index = y * surface->pitch / 4 + x;

    // Obtain the pixel value from the array of pixels in the surface
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[index];
}

// Obtain a pixel colour component from an SDL surface
Uint8 get_pixel_component(SDL_Surface *surface, int x, int y, char component)
{
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
    {
        Uint8 *pixelAddress = (Uint8 *)surface->pixels + y * surface->pitch
            + x * surface->format->BytesPerPixel;
        Uint32 pixelValue = *(Uint32 *)pixelAddress;

        Uint8 r, g, b;
        SDL_GetRGB(pixelValue, surface->format, &r, &g, &b);

        switch (component)
        {
        case 'r':
            return r;
        case 'g':
            return g;
        case 'b':
            return b;
        default:
            return 0;
        }
    }
    return 0;
}

void detectContours(SDL_Surface *surface, SDL_Surface *surface2, int val)
{
    int width = surface->w;
    int height = surface->h;

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            if (get_pixel_component(surface, x, y, 'r') == 255)
            {
                int hasBlackNeighbor = 0;

                for (int i = 1; i <= val; i++)
                {
                    if (get_pixel_component(surface, x - i, y, 'r') == 0
                        || get_pixel_component(surface, x + i, y, 'r') == 0
                        || get_pixel_component(surface, x, y - i, 'r') == 0
                        || get_pixel_component(surface, x, y + i, 'r') == 0
                        || get_pixel_component(surface, x - i, y - i, 'r') == 0
                        || get_pixel_component(surface, x + i, y - i, 'r') == 0
                        || get_pixel_component(surface, x - i, y + i, 'r') == 0
                        || get_pixel_component(surface, x + i, y + i, 'r') == 0)
                    {
                        hasBlackNeighbor = 1;
                        break;
                    }
                }

                if (hasBlackNeighbor == 1)
                {
                    set_pixel(surface2, x, y, 255, 255, 255);
                }
                else
                {
                    set_pixel(surface2, x, y, 0, 0, 0);
                }
            }
        }
    }
}

// Restore a binary image
void restore_binary_image(SDL_Surface *binarySurface)
{
    int width = binarySurface->w;
    int height = binarySurface->h;

    // Create a copy of the binary surface
    SDL_Surface *restoredSurface = SDL_CreateRGBSurfaceWithFormat(
        0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_BlitSurface(binarySurface, NULL, restoredSurface, NULL);

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            // If the pixel is white, check for neighboring black pixels
            if (get_pixel_component(binarySurface, x, y, 'r') == 255)
            {
                int blackCount = 0;

                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        if (get_pixel_component(binarySurface, x + i, y + j,
                                                'r')
                            == 255)
                        {
                            blackCount++;
                        }
                    }
                }

                // If there are enough black neighbors, set the pixel to black
                if (blackCount >= 5)
                {
                    set_pixel(restoredSurface, x, y, 255, 255, 255);
                }
            }
        }
    }

    // Copy the restored surface back to the original binary surface
    SDL_BlitSurface(restoredSurface, NULL, binarySurface, NULL);

    // Free the memory used by the restored surface
    SDL_FreeSurface(restoredSurface);
}

// Set white pixels on the image border to black
void set_border_to_black(SDL_Surface *surface)
{
    int width = surface->w;
    int height = surface->h;

    // Top border
    for (int x = 0; x < width; x++)
    {
        if (get_pixel_component(surface, x, 0, 'r') == 255)
        {
            set_pixel(surface, x, 0, 0, 0, 0);
        }
    }

    // Bottom border
    for (int x = 0; x < width; x++)
    {
        if (get_pixel_component(surface, x, height - 1, 'r') == 255)
        {
            set_pixel(surface, x, height - 1, 0, 0, 0);
        }
    }

    // Left border
    for (int y = 0; y < height; y++)
    {
        if (get_pixel_component(surface, 0, y, 'r') == 255)
        {
            set_pixel(surface, 0, y, 0, 0, 0);
        }
    }

    // Right border
    for (int y = 0; y < height; y++)
    {
        if (get_pixel_component(surface, width - 1, y, 'r') == 255)
        {
            set_pixel(surface, width - 1, y, 0, 0, 0);
        }
    }
}

void analyse(int angle, char *img)
{ /*
     if (argc != 3)
         errx(EXIT_FAILURE, "Usage: image-file rotation_angle");

     if (SDL_Init(SDL_INIT_VIDEO) != 0)
         errx(EXIT_FAILURE, "SDL_Init: %s", SDL_GetError());
 */
    // orient(name, atof(argv[2]));

    // toCut(name, );

    /*if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file rotation_angle");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "SDL_Init: %s", SDL_GetError());*/

    SDL_WindowFlags wF = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    int wP = SDL_WINDOWPOS_CENTERED;

    //    int angle = atof(argv[2]);
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

    SDL_Surface *surface_r = load_image(img);
    SDL_Surface *surface_r2 = load_image(img);

    if (strcmp(img, "img_01.png") == 0)
    {
        surface_r = surface_to_grayscale(surface_r);
        threshold_image(surface_r, 0, 0);
        surface_r2 = load_image(img);
        detectContours(surface_r, surface_r2, 1);
        set_border_to_black(surface_r2);

        // Create a texture from the grayscale surface
        SDL_Texture *texture_r = SDL_CreateTextureFromSurface(r_r, surface_r2);
        if (texture_r == NULL)
            errx(EXIT_FAILURE, "SDL_CreateTextureFromSurface: %s",
                 SDL_GetError());
        rotation(r_r, texture_r, angle, "img_analyse.png");
    }
    else if (strcmp(img, "img_02.png") == 0)
    {
        threshold_image(surface_r2, 40, 1);
        threshold_image(surface_r, 40, 1);
    }
    else if (strcmp(img, "img_03.png") == 0)
    {
        threshold_image(surface_r2, 0, 1);
        threshold_image(surface_r, 0, 1);
    }
    else if (strcmp(img, "img_04.png") == 0)
    {
        threshold_image(surface_r, 100, 1);
        threshold_image(surface_r2, 100, 1);
    }
    else if (strcmp(img, "img_05.png") == 0)
    {
        threshold_image(surface_r, 50, 1);
        threshold_image(surface_r2, 50, 1);
    }
    else if (strcmp(img, "img_06.png") == 0)
    {
        threshold_image(surface_r, 14, 1);
        threshold_image(surface_r2, 14, 1);
    }
    else
    {
        threshold_image(surface_r, 0, 1);
        threshold_image(surface_r2, 0, 1);
    }

    if (surface_r == NULL)
        errx(EXIT_FAILURE, "load_image: %s", SDL_GetError());

    //   IMG_SavePNG(surface_r, "surface1.png");
    // IMG_SavePNG(surface_r2, "surface2.png");
    detectContours(surface_r, surface_r2, 1);

    if (angle == 0)
        IMG_SavePNG(surface_r2, "img_analyse.png");
    else
    {
        // Create a texture from the grayscale surface
        SDL_Texture *texture_r = SDL_CreateTextureFromSurface(r_r, surface_r2);
        if (texture_r == NULL)
            errx(EXIT_FAILURE, "SDL_CreateTextureFromSurface: %s",
                 SDL_GetError());
        rotation(r_r, texture_r, angle, "img_analyse.png");
        SDL_DestroyTexture(texture_r);
    }

    SDL_DestroyRenderer(r_r);
    SDL_DestroyWindow(w_r);
    SDL_Quit();
}
