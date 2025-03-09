
#include "../include/header.h"

double sigmoid(double z)
{
    return 1 / (1 + exp(-z));
}

double magnitude(double *vect, size_t len)
{
    double sum = 0;
    while (len > 0)
    {
        sum += vect[len - 1] * vect[len - 1];
        len--;
    }
    return sqrt(sum);
}

double *vector_sum(double *v1, double *v2, size_t n)
{
    double *v3 = (double *)malloc(n * sizeof(double));
    while (n > 0)
    {
        v3[n - 1] = v1[n - 1] + v2[n - 1];
        n--;
    }
    return v3;
}

double *vector_sub(double *v1, double *v2, size_t n)
{
    double *v3 = (double *)malloc(n * sizeof(double));
    while (n > 0)
    {
        v3[n - 1] = v1[n - 1] - v2[n - 1];
        n--;
    }
    return v3;
}

void swap(double **a, double **b)
{
    double *temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(double **inputs, double **refs, size_t size)
{
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Fisher-Yates shuffle algorithm
    for (size_t i = size - 1; i > 0; i--)
    {
        size_t j = rand() % (i + 1);
        swap(&inputs[i], &inputs[j]);
        swap(&refs[i], &refs[j]);
    }
}
