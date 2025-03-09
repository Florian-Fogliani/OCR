#include "../include/header.h"

void save(network *net, char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        write(2, "can't open file\n", 16);
        return;
    }
    int hidden_layer_size = L2;
    fwrite(&hidden_layer_size, sizeof(int), 1, file);
    for (size_t i = 0; i < L2; i++)
    {
        for (size_t j = 0; j < L1; j++)
            fwrite(&(net->w2[i][j]), sizeof(double), 1, file);
        fwrite(&(net->b2[i]), sizeof(double), 1, file);
    }
    for (size_t i = 0; i < L3; i++)
    {
        for (size_t j = 0; j < L2; j++)
            fwrite(&(net->w3[i][j]), sizeof(double), 1, file);
        fwrite(&(net->b3[i]), sizeof(double), 1, file);
    }
}

void load(network *net, char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        write(2, "can't open file\n", 16);
        return;
    }
    int hidden_layer_size;
    fread(&hidden_layer_size, sizeof(int), 1, file);
    if (hidden_layer_size != L2)
    {
        write(2, "the hidden layer size doesn't match\n", 37);
        return;
    }

    net->w2 = (double **)malloc(L2 * sizeof(double *));
    net->b2 = (double *)malloc(L2 * sizeof(double));
    net->z2 = (double *)malloc(L2 * sizeof(double));
    net->a2 = (double *)malloc(L2 * sizeof(double));

    for (size_t i = 0; i < L2; i++)
    {
        net->w2[i] = malloc(L1 * sizeof(double));
        for (size_t j = 0; j < L1; j++)
            fread(&(net->w2[i][j]), sizeof(double), 1, file);
        fread(&(net->b2[i]), sizeof(double), 1, file);
    }

    net->w3 = (double **)malloc(L3 * sizeof(double *));
    net->b3 = (double *)malloc(L3 * sizeof(double));
    net->z3 = (double *)malloc(L3 * sizeof(double));
    net->a3 = (double *)malloc(L3 * sizeof(double));

    for (size_t i = 0; i < L3; i++)
    {
        net->w3[i] = malloc(L2 * sizeof(double));
        for (size_t j = 0; j < L2; j++)
            fread(&(net->w3[i][j]), sizeof(double), 1, file);
        fread(&(net->b3[i]), sizeof(double), 1, file);
    }
}
