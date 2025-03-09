
#include "../include/header.h"

void forwardPropagation(network *net, double *input)
{
    for (size_t j = 0; j < L2; j++)
    {
        double sum = 0;
        for (size_t i = 0; i < L1; i++)
            sum += net->w2[j][i] * input[i];

        net->z2[j] = sum + net->b2[j];
        net->a2[j] = sigmoid(net->z2[j]);
    }

    for (size_t j = 0; j < L3; j++)
    {
        double sum = 0;
        for (size_t i = 0; i < L2; i++)
            sum += net->w3[j][i] * net->a2[i];

        net->z3[j] = sum + net->b3[j];
        net->a3[j] = sigmoid(net->z3[j]);
    }
}

int evaluate(network *net, double **inputs, double **refs, double threshold)
{
    size_t i = 0;
    int success = 0;
    while (i < TRAININGS)
    {
        forwardPropagation(net, inputs[i]);
        double *diff = vector_sub(refs[i], net->a3, L3);
        double mag = magnitude(diff, L3);
        if (mag * mag < threshold)
            success++;
        free(diff);
        i++;
    }
    return success;
}

void backPropagation(network *net, double *input, double *ref, double rate)
{
    double *coefL3 = malloc(L3 * sizeof(double));
    double *coefL2 = malloc(L2 * sizeof(double));
    for (size_t i = 0; i < L3; i++)
    {
        coefL3[i] = 2 * (net->a3[i] - ref[i]) * net->a3[i] * (1 - net->a3[i]);
        for (size_t j = 0; j < L2; j++)
            net->w3[i][j] -= rate * coefL3[i] * net->a2[j];
        net->b3[i] -= rate * coefL3[i];
    }

    for (size_t j = 0; j < L2; j++)
    {
        double sum = 0;
        for (size_t i = 0; i < L3; i++)
            sum += coefL3[i] * net->w3[i][j];
        coefL2[j] = sum;
    }

    for (size_t i = 0; i < L2; i++)
    {
        coefL2[i] *= net->a2[i] * (1 - net->a2[i]);
        for (size_t j = 0; j < L1; j++)
            net->w2[i][j] -= rate * coefL2[i] * input[j];
        net->b2[i] -= rate * coefL2[i];
    }
    free(coefL3);
    free(coefL2);
}

void createBatch(double **bInputs, double **bRefs, double **inputs,
                 double **refs, size_t n)
{
    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        size_t in = (i * n) % TRAININGS;
        for (size_t j = 0; j < L1; j++)
            bInputs[i][j] = inputs[in][j];
        for (size_t j = 0; j < L3; j++)
            bRefs[i][j] = refs[in][j];
    }
}

void sgd(network *net, double **inputs, double **refs)
{
    double **bInputs = malloc(BATCH_SIZE * sizeof(void *));
    double **bRefs = malloc(BATCH_SIZE * sizeof(void *));
    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        bInputs[i] = malloc(L1 * sizeof(double));
        bRefs[i] = malloc(L3 * sizeof(double));
    }

    shuffle(inputs, refs, TRAININGS);
    shuffle(inputs, refs, TRAININGS);
    shuffle(inputs, refs, TRAININGS);
    double rate = RATE;
    for (size_t e = 0; e < EPOCHS; e++)
    {
        createBatch(bInputs, bRefs, inputs, refs, e);
        for (size_t im = 0; im < BATCH_SIZE; im++)
        {
            forwardPropagation(net, bInputs[im]);
            backPropagation(net, bInputs[im], bRefs[im], rate);
        }
        if (e % 100 == 0)
        {
            printf("EPOCH %zu : %f (rate is %lf)\n", e,
                   (double)evaluate(net, inputs, refs, 0.1) / TRAININGS * 100.0,
                   rate);
            rate *= DECAY;
        }
    }

    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        free(bInputs[i]);
        free(bRefs[i]);
    }
    free(bInputs);
    free(bRefs);
}
