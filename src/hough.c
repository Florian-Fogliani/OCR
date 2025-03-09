#include "hough.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

void invertColors(SDL_Surface *surface)
{
    if (surface == NULL)
    {
        fprintf(stderr, "Surface is NULL\n");
        return;
    }

    SDL_LockSurface(surface);

    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            Uint32 pixel =
                *((Uint32 *)surface->pixels + y * surface->pitch / 4 + x);
            Uint8 r, g, b, a;

            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

            if (r > 128 && g > 128 && b > 128)
            {
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                r = 255;
                g = 255;
                b = 255;
            }

            *((Uint32 *)surface->pixels + y * surface->pitch / 4 + x) =
                SDL_MapRGBA(surface->format, r, g, b, a);
        }
    }

    SDL_UnlockSurface(surface);
}

int *Init_Mat(const int R)
{
    int *res = calloc(180 * (R * 2), sizeof(int));
    return res;
}

void Free_Mat(int *mat)
{
    free(mat);
}

void Free_Lines(struct Line *h, struct Line *v)
{
    free(h);
    free(v);
}

int Fill_Mat(SDL_Surface *img, int *mat, const int diag_size)
{
    Uint32 *pixels = img->pixels;
    SDL_PixelFormat *format = img->format;
    Uint8 r, g, b;
    int max = 0;
    for (int h = 0; h < img->h; h++)
    {
        for (int w = 0; w < img->w; w++)
        {
            SDL_GetRGB(pixels[(h * img->w) + w], format, &r, &g, &b);
            if (r != 0 && g != 0 && b != 0)
            {
                for (int theta = -90; theta <= 90; theta++)
                {
                    int p = (int)(w * cos((theta)*M_PI / 180)
                                  + h * sin((theta)*M_PI / 180));
                    mat[(p + diag_size) * (180) + (theta + 90)] += 1;
                    if (mat[(p + diag_size) * (180) + (theta + 90)] > max)
                    {
                        max = mat[(p + diag_size) * 180 + (theta + 90)];
                    }
                }
            }
        }
    }
    return max;
}

double Calculate_Diagonal(SDL_Surface *img)
{
    double p1 = pow((double)img->w, 2);
    double p2 = pow((double)img->h, 2);
    double res = sqrt(p1 + p2);
    return res;
}

void Debug(int *mat, const int diag_size, char *img, int w, int h, int max)
{
    SDL_Window *window =
        SDL_CreateWindow("Debug", 0, 0, w, h, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 238, 130, 238, 255);
    SDL_Texture *texture = IMG_LoadTexture(renderer, img);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    int seuil = max * 0.4;
    for (int p = 0; p <= diag_size * 2; p++)
    {
        for (int t = 0; t <= 180; t++)
        {
            if (mat[p * (180) + t] > seuil)
            {
                drawLine(renderer, p - diag_size, t - 90, w, h);
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            return;
        }
    }
}
void drawLine(SDL_Renderer *renderer, int rho, int theta, int width, int height)
{
    if (theta == 0)
    {
        SDL_RenderDrawLine(renderer, rho, 1, rho, height);
    }
    else
    {
        double angleRad = theta * M_PI / 180;
        int x0 = 0;
        int y0 = rho / sin(angleRad);
        int x1 = width - 1;
        int y1 = x1 * -cos(angleRad) / sin(angleRad) + y0;
        SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    }
}

void Debug_GetLines(struct Line *horizontals, struct Line *verticals,
                    int *size_horizontals, int *size_verticals, int w, int h,
                    char *img, int type)
{
    SDL_Window *window =
        SDL_CreateWindow("Debug GetLines", 0, 0, w, h, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 238, 130, 238, 255);
    SDL_Texture *texture = IMG_LoadTexture(renderer, img);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    if (type == 0)
    {
        for (int i = 0; i < (*size_horizontals); i++)
        {
            int p = horizontals[i].rho;
            int t = horizontals[i].theta;
            drawLine(renderer, p, t, w, h);
        }
    }
    else
    {
        for (int i = 0; i < (*size_verticals); i++)
        {
            int p = verticals[i].rho;
            int t = verticals[i].theta;
            drawLine(renderer, p, t, w, h);
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            return;
        }
    }
}

void GetLines(int *mat, int diag_size, int max, struct Line **horizontals,
              struct Line **verticals, int *size_horizontals,
              int *size_verticals, SDL_Surface *img)
{
    int thresold = max * 0.4;
    int thresold_horizon = 5;
    for (int p = 0; p <= diag_size * 2; p++)
    {
        for (int t = 0; t <= 180; t++)
        {
            if (mat[p * (180) + t] > thresold)
            {
                int real_p = p - diag_size;
                int real_t = t - 90;
                if (abs(real_t) > thresold_horizon && abs(real_t) < 130
                    && real_p + img->h > 10) // Horizontals Lines
                {
                    struct Line to_save = { real_p, real_t };
                    Insert_Sort(horizontals, size_horizontals, &to_save);
                }
                if (abs(real_t) <= thresold_horizon) // Verticals Lines
                {
                    struct Line to_save = { real_p, real_t };
                    Insert_Sort(verticals, size_verticals, &to_save);
                }
            }
        }
    }
}

void Insert_Sort(struct Line **tab, int *len, struct Line *line)
{
    int i = 0;
    while (i < *len && line->rho > (*tab + i)->rho)
        i++;
    (*len)++;
    *tab = realloc(*tab, (*len) * sizeof(struct Line));
    int index = i;
    i = (*len) - 1;
    ;
    while (i > index)
    {
        (*tab + i)->rho = (*tab + i - 1)->rho;
        i--;
    }
    (*tab)[index] = *line;
}

struct Point GetIntersec(int p1, int o1, int p2, int o2)
{
    struct Point intersec;
    if (p1 == p2)
    {
        printf("Error : droites parallèles");
    }

    intersec.x = (o2 - o1) / (p1 - p2);
    intersec.y = p1 * intersec.x + o1;
    return intersec;
}

void SaveGrid(SDL_Surface *img, int nb_l, int nb_col, struct Point bord_up,
              struct Point bord_down)
{
    int x1 = bord_up.x;
    int y1 = bord_up.y;
    int x2 = bord_down.x;
    int y2 = bord_down.y;
    int largeur = x2 - x1;
    int hauteur = y2 - y1;
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = largeur;
    rect.h = hauteur;
    SDL_Surface *capture =
        SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0, 0, 0, 0);
    char name[20];
    sprintf(name, "mat_%d_%d", nb_l, nb_col);
    SDL_BlitSurface(img, &rect, capture, NULL);
    IMG_SavePNG(capture, name);
    SDL_FreeSurface(capture);
}

void SaveCas(SDL_Surface *img, int nb_l, int nb_col, struct Point bord_up,
             struct Point bord_down)
{
    int x1 = bord_up.x;
    int y1 = bord_up.y;
    int x2 = bord_down.x;
    int y2 = bord_down.y;
    int largeur = x2 - x1;
    int hauteur = y2 - y1;
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = largeur;
    rect.h = hauteur;
    SDL_Surface *capture =
        SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0, 0, 0, 0);
    char name[20];
    sprintf(name, "mat_%d_%d.png", nb_l, nb_col);
    SDL_BlitSurface(img, &rect, capture, NULL);
    // invertColors(capture);
    // capture = zoom(capture,0.9,capture->w/2,0);
    IMG_SavePNG(capture, name);
    SDL_FreeSurface(capture);
}

void Cut(struct Line **horizontals, struct Line **verticals, int *size_h,
         int *size_v, SDL_Surface *img)
{
    struct Line *hor = (struct Line *)get_10_lines(*(horizontals), *size_h, 10);
    struct Line *ver = (struct Line *)get_10_lines(*(verticals), *size_v, 10);

    if (ver == NULL)
    {
        printf("Erreur Verti");
    }
    if (hor == NULL)
    {
        printf("Erreur Hori");
    }
    int x1 = ver[0].rho;
    int y1 = hor[0].rho;
    if (hor[0].rho < 0)
    {
        y1 = img->h + hor[0].rho;
    }
    else
    {
        y1 = hor[0].rho;
    }
    int x2 = ver[9].rho;
    int y2 = hor[9].rho;
    if (hor[9].rho < 0)
    {
        y2 = img->h + hor[9].rho;
    }
    else
    {
        y2 = hor[9].rho;
    }
    if (y2 > img->h)
    {
        printf("===Y Superior== \n");
        y2 = img->h - 1;
    }
    if (x2 > img->w)
    {
        printf("====X Superior === \n");
        x2 = img->w - 1;
    }
    struct Point up = { x1, y1 };
    struct Point down = { x2, y2 };
    SaveGrid(img, 0, 0, up, down);
}
void CutFinale(SDL_Surface *img)
{
    SDL_Surface *imgf = img;
    int step_w = imgf->w / 9;
    int step_h = imgf->h / 9;
    int a = 0;
    int b = 0;
    for (int y = 0; y <= imgf->h - step_h; y += step_h)
    {
        for (int x = 0; x <= imgf->w - step_w; x += step_w)
        {
            struct Point up = { x, y };
            struct Point down = { x + step_w, y + step_h };
            SaveCas(imgf, a + 1, b + 1, up, down);
            b++;
        }
        a++;
        b = 0;
    }
}
struct Line *get_10_refs(struct Line *tab, int len, int threshold, int ref,
                         int i)
{
    struct Line *ten = calloc(1, sizeof(struct Line));

    int a = i;
    int b = a + 1;
    int n = 0;
    int diff = tab[b].rho - tab[a].rho;

    while (diff <= 50)
    {
        b++;
        diff = tab[b].rho - tab[a].rho;
    }

    while (b < len && ref - diff >= -threshold)
    {
        if (ref - diff <= threshold)
        {
            n++;
            ten = realloc(ten, n * sizeof(struct Line));
            ten[n - 1] = tab[a];
            a = b;
            b = a + 1;
            ref = diff;
            if (n == 10)
                return ten;
        }
        else
            b++;
        diff = tab[b].rho - tab[a].rho;
    }

    if (n == 9 && ref - tab[len - 1].rho - tab[a].rho <= threshold)
    {
        ten = realloc(ten, 10 * sizeof(struct Line));
        if (a < len)
        {
            ten[n] = tab[a];
        }
        else
        {
            ten[n] = tab[len - 1];
        }
        return ten;
    }

    return NULL;
}

struct Line *get_10_lines(struct Line *tab, int len, int threshold)
{
    struct Line *ten;

    int i = 0;
    int j = 0;
    int max = len - 10;

    while (i <= max)
    {
        j = i + 1;
        while (j <= len)
        {
            int ref = tab[j].rho - tab[i].rho;
            ten = get_10_refs(tab, len, threshold, ref, i);
            if (ten != NULL)
                return ten;
            j++;
        }
        i++;
    }
    return NULL;
}
