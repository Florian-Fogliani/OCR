
#include "../include/header.h"

void init_network(network *net)
{
    srand(17);
    size_t i = 0;
    size_t j = 0;
    net->w2 = (double **)malloc(L2 * sizeof(double *));
    net->b2 = (double *)malloc(L2 * sizeof(double));
    net->z2 = (double *)malloc(L2 * sizeof(double));
    net->a2 = (double *)malloc(L2 * sizeof(double));

    while (j < L2)
    {
        net->w2[j] = (double *)malloc(L1 * sizeof(double));
        i = 0;
        while (i < L1)
        {
            net->w2[j][i] = 2 * (double)rand() / RAND_MAX - 1;
            i++;
        }
        net->b2[j] = 2 * (double)rand() / RAND_MAX - 1;
        j++;
    }

    i = 0;
    j = 0;
    net->w3 = (double **)malloc(L3 * sizeof(double *));
    net->b3 = (double *)malloc(L3 * sizeof(double));
    net->z3 = (double *)malloc(L3 * sizeof(double));
    net->a3 = (double *)malloc(L3 * sizeof(double));

    while (j < L3)
    {
        net->w3[j] = (double *)malloc(L2 * sizeof(double));
        i = 0;
        while (i < L2)
        {
            net->w3[j][i] = 2 * (double)rand() / RAND_MAX - 1;
            i++;
        }
        net->b3[j] = 2 * (double)rand() / RAND_MAX - 1;
        j++;
    }
}

void free_network(network *net)
{
    size_t j = 0;
    while (j < L2)
    {
        free(net->w2[j]);
        j++;
    }
    free(net->w2);
    free(net->b2);
    free(net->z2);
    free(net->a2);
    j = 0;
    while (j < L3)
    {
        free(net->w3[j]);
        j++;
    }
    free(net->w3);
    free(net->b3);
    free(net->z3);
    free(net->a3);
}

void print_network(network *net)
{
    size_t i = 0;
    size_t j = 0;
    while (j < L2)
    {
        i = 0;
        while (i < L1)
        {
            printf(" %10lf |", net->w2[j][i]);
            i++;
        }
        printf(" === %10lf\n", net->b2[j]);
        j++;
    }
    printf("\n\n");
    j = 0;
    while (j < L3)
    {
        i = 0;
        while (i < L2)
        {
            printf(" %10lf |", net->w3[j][i]);
            i++;
        }
        printf(" === %10lf\n", net->b3[j]);
        j++;
    }
    printf("\n");
}

network *network_copy(network *net)
{
    network *cpy = (network *)malloc(sizeof(network));
    size_t i = 0;
    size_t j = 0;
    cpy->w2 = (double **)malloc(L2 * sizeof(double *));
    cpy->b2 = (double *)malloc(L2 * sizeof(double));
    cpy->z2 = (double *)malloc(L2 * sizeof(double));
    cpy->a2 = (double *)malloc(L2 * sizeof(double));

    while (j < L2)
    {
        i = 0;
        cpy->w2[j] = malloc(L1 * sizeof(double));
        while (i < L1)
        {
            cpy->w2[j][i] = net->w2[j][i];
            i++;
        }
        cpy->b2[j] = net->b2[j];
        j++;
    }

    j = 0;
    cpy->w3 = (double **)malloc(L3 * sizeof(double *));
    cpy->b3 = (double *)malloc(L3 * sizeof(double));
    cpy->z3 = (double *)malloc(L3 * sizeof(double));
    cpy->a3 = (double *)malloc(L3 * sizeof(double));
    while (j < L3)
    {
        i = 0;
        cpy->w3[j] = malloc(L2 * sizeof(double));
        while (i < L2)
        {
            cpy->w3[j][i] = net->w3[j][i];
            i++;
        }
        cpy->b3[j] = net->b3[j];
        j++;
    }
    return cpy;
}
