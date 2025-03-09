#include "draw.h"

#include "solver.h"

// Initial width and height of the window.
int INIT_WIDTH = 640;
int INIT_HEIGHT = 400;
int *grid;
int *deb;

// Draws two diagonals.
//
// renderer: Renderer to draw on.

void draw_one(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + w / 2, y + h / 4, x + w / 2,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + w / 2, y + h / 4, x + w / 4, y + h / 2);
    SDL_RenderDrawLine(renderer, x + w / 3, y + 3 * h / 4, x + 2 * w / 3,
                       y + 3 * h / 4);
}

void draw_two(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 3 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 3 * w / 8,
                       y + 3 * h / 4);
}

void draw_tree(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 3 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
}

void draw_four(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 3 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
}

void draw_five(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 3 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 3 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
}
void draw_six(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 3 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 3 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
}

void draw_seven(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
}

void draw_eight(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 3 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 3 * w / 8,
                       y + 3 * h / 4);
}

void draw_nine(SDL_Renderer *renderer, int w, int h, int x, int y)
{
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 2 * h / 4, x + 5 * w / 8,
                       y + 2 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + 3 * h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 5 * w / 8, y + h / 4, x + 5 * w / 8,
                       y + 3 * h / 4);
    SDL_RenderDrawLine(renderer, x + 3 * w / 8, y + h / 4, x + 3 * w / 8,
                       y + 2 * h / 4);
}

void draw_grid(SDL_Renderer *renderer, int w, int h)
{
    // Clears the renderer (sets the background to white).
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Sets the color for drawing operations to black.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draws the two diagonals.
    SDL_Rect box;
    box.w = w / (4 * 3);
    box.h = h / (4 * 3);
    for (int i = 0; i < 10; i++)
    {
        if (i % 3 == 0)
        {
            SDL_RenderDrawLine(renderer, w / 8, h / 8 + (i * box.h) - 1,
                               w - w / 8, h / 8 + (i * box.h) - 1);
            SDL_RenderDrawLine(renderer, w / 8 + (i * box.w) - 1, h / 8,
                               w / 8 + (i * box.w) - 1, h - h / 8);
            SDL_RenderDrawLine(renderer, w / 8, h / 8 + (i * box.h) + 1,
                               w - w / 8, h / 8 + (i * box.h) + 1);
            SDL_RenderDrawLine(renderer, w / 8 + (i * box.w) + 1, h / 8,
                               w / 8 + (i * box.w) + 1, h - h / 8);
        }
        SDL_RenderDrawLine(renderer, w / 8, h / 8 + (i * box.h), w - w / 8,
                           h / 8 + (i * box.h));
        SDL_RenderDrawLine(renderer, w / 8 + (i * box.w), h / 8,
                           w / 8 + (i * box.w), h - h / 8);
    }
}

void draw(SDL_Renderer *renderer, int w, int h)
{
    draw_grid(renderer, w, h);
    SDL_Rect box;
    box.w = w / (4 * 3);
    box.h = h / (4 * 3);
    int p = 0;
    while (grid[p])
    {
        int i = p / 9;
        int j = p % 9;
        if (deb[p])
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        switch (grid[p])
        {
        case 1:
            draw_one(renderer, box.w, box.h, w / 8 + j * box.w,
                     h / 8 + i * box.h);
            break;
        case 2:
            draw_two(renderer, box.w, box.h, w / 8 + j * box.w,
                     h / 8 + i * box.h);
            break;
        case 3:
            draw_tree(renderer, box.w, box.h, w / 8 + j * box.w,
                      h / 8 + i * box.h);
            break;
        case 4:
            draw_four(renderer, box.w, box.h, w / 8 + j * box.w,
                      h / 8 + i * box.h);
            break;
        case 5:
            draw_five(renderer, box.w, box.h, w / 8 + j * box.w,
                      h / 8 + i * box.h);
            break;
        case 6:
            draw_six(renderer, box.w, box.h, w / 8 + j * box.w,
                     h / 8 + i * box.h);
            break;
        case 7:
            draw_seven(renderer, box.w, box.h, w / 8 + j * box.w,
                       h / 8 + i * box.h);
            break;
        case 8:
            draw_eight(renderer, box.w, box.h, w / 8 + j * box.w,
                       h / 8 + i * box.h);
            break;
        case 9:
            draw_nine(renderer, box.w, box.h, w / 8 + j * box.w,
                      h / 8 + i * box.h);
            break;
        default:
            break;
        }
        p += 1;
    }
    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer *renderer)
{
    // Width and height of the window.
    int w = INIT_WIDTH;
    int h = INIT_HEIGHT;

    // Draws the diagonals (first draw).
    draw(renderer, w, h);

    // Creates a variable to get the events.
    SDL_Event event;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        // If the "quit" button is pushed, ends the event loop.
        case SDL_QUIT:
            return;

        // If the window is resized, updates and redraws the diagonals.
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                w = event.window.data1;
                h = event.window.data2;
                INIT_WIDTH = w;
                INIT_HEIGHT = h;
                draw(renderer, w, h);
            }
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        err(1, "wrong parameter");
    int **res = do_all_solve(argc, argv);
    grid = res[0];
    deb = res[1];
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window *window =
        SDL_CreateWindow("Plain Window", 0, 0, INIT_WIDTH, INIT_HEIGHT,
                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Dispatches the events.

    // Save the surface as a PNG image
    event_loop(renderer);
    int pitch = sizeof(Uint32) * INIT_WIDTH;
    Uint32 *pixels = malloc(pitch * INIT_HEIGHT);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels,
                         pitch);
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(
        pixels, INIT_WIDTH, INIT_HEIGHT, 32, pitch, SDL_PIXELFORMAT_RGBA8888);
    SDL_SaveBMP(surface, "solve.result.bmp");

    // Destroys the objects.
    free(grid);
    free(deb);
    free(res);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
