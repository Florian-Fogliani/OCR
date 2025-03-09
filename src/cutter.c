#include <err.h>
#include <sys/wait.h>
#include <unistd.h>

#include "hough.h"
#include "stdlib.h"

SDL_Surface *load_image(const char *path)
{
    SDL_Surface *temp = IMG_Load(path);
    if (temp == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *res =
        SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888, 0);
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(temp);
    return res;
}

void print_param(struct Line *l, int s)
{
    int i = 0;
    while (i < s)
    {
        printf("Rho %i, Theta %i \n", l[i].rho, l[i].theta);
        i++;
    }
}

int main(int argc, char **argv)
{
    if (strcmp(argv[1], "clean") == 0)
    {
        char path[1024];
        getcwd(path, sizeof(path));
        pid_t pid = fork();
        if (pid == 0)
        {
            char nb[] = { '/', 'm', 'a', 't', '_', 48, '_', 48, '\0' };
            char *arg[] = { "rm", strcat(path, nb), NULL };
            execvp("rm", arg);
            return 0;
        }
        else
        {
            wait(NULL);
        }
        for (int i = 1; i <= 9; i++)
        {
            for (int y = 1; y <= 9; y++)
            {
                pid_t pid = fork();
                if (pid == 0)
                {
                    char nb[] = { '/',    'm', 'a', 't', '_', i + 48, '_',
                                  y + 48, '.', 'p', 'n', 'g', '\0' };
                    char *arg[] = { "rm", strcat(path, nb), NULL };
                    execvp("rm", arg);
                    return 0;
                }
                else if (pid > 0)
                {
                    wait(NULL);
                }
            }
        }
        return 0;
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    SDL_Surface *surface = load_image(argv[1]);
    if (surface == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    if (argc == 3)
    {
        int diag_size = (int)Calculate_Diagonal(surface);
        int *mat = Init_Mat((const int)diag_size);
        int max = Fill_Mat(surface, mat, (const int)diag_size);
        struct Line *horizontals = calloc(1, sizeof(struct Line));
        struct Line *verticals = calloc(1, sizeof(struct Line));
        int size_h = 1;
        int size_v = 1;
        GetLines(mat, diag_size, max, &horizontals, &verticals, &size_h,
                 &size_v, surface);
        SDL_Surface *to_cut = load_image(argv[2]);
        Cut(&horizontals, &verticals, &size_h, &size_v, to_cut);
        SDL_Surface *loaded = load_image("mat_0_0");
        to_cut = SDL_CreateRGBSurface(
            0, 252, 252, 32, loaded->format->Rmask, loaded->format->Gmask,
            loaded->format->Bmask, loaded->format->Amask);
        SDL_Rect r = { 0, 0, 253, 253 };
        SDL_BlitScaled(loaded, NULL, to_cut, &r);
        CutFinale(to_cut);
        Free_Mat(mat);
        Free_Lines(horizontals, verticals);
    }
    else
    {
        const int diag_size = (int)Calculate_Diagonal(surface);
        int *mat = Init_Mat(diag_size);
        int max = Fill_Mat(surface, mat, diag_size);
        if (strcmp(argv[3], "debug") == 0)
        {
            Debug(mat, diag_size, argv[1], surface->w, surface->h, max);
        }
        else
        {
            int type_debug = 0;
            if (strcmp(argv[3], "verticals") == 0)
            {
                type_debug = 1;
            }
            else if (strcmp(argv[3], "horizontals") == 0)
            {
                type_debug = 0;
            }
            else
            {
                return 1;
            }
            struct Line *horizontals = calloc(1, sizeof(struct Line));
            struct Line *verticals = calloc(1, sizeof(struct Line));
            int size_h = 1;
            int size_v = 1;
            GetLines(mat, diag_size, max, &horizontals, &verticals, &size_h,
                     &size_v, surface);
            printf("============BEFORE SELECT HORI============");
            print_param(horizontals, size_h);
            printf("============AFTER SELECT VERTI============");
            print_param(verticals, size_v);
            printf("=======GET HORI======\n");
            struct Line *h = get_10_lines(horizontals, size_h, 10);
            printf("========GET VERTI======\n");
            struct Line *v = get_10_lines(verticals, size_v, 10);
            int sh = 10;
            int sv = 10;
            if (h == NULL || v == NULL)
                printf("ERROR");

            printf("================ VERTICALS ==================  \n");
            print_param(v, sv);
            printf("================ HORIZONTALS ================ \n");
            print_param(h, sh);
            Debug_GetLines(h, v, &sh, &sv, surface->w, surface->h, argv[1],
                           type_debug);
        }
    }
    return 0;
}
