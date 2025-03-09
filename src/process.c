
#include <arpa/inet.h>

#include "include/header.h"

void displayimage(double *image)
{
    for (size_t i = 0; i < 28; i++)
    {
        for (size_t j = 0; j < 28; j++)
        {
            // printf("%lf", image[i * 28 + j]);
            if (image[i * 28 + j] > 128)
                printf("%c%c", 254, 254);
            else
                printf("  ");
        }
        printf("\n");
    }
}

int outputToInt(network *net)
{
    double max = 0.0;
    int n = 0;
    for (int j = 0; j < 10; j++)
    {
        if (net->a3[j] > max)
        {
            max = net->a3[j];
            n = j;
        }
    }
    return n;
}

void test(network *net, double **inputs, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        n %= TRAININGS;
        displayimage(inputs[i]);
        forwardPropagation(net, inputs[i]);
        printf("result ====> %d\n", outputToInt(net));
    }
}

double imagesum(double *image)
{
    double sum = 0;
    for (size_t i = 8; i < 20; i++)
    {
        for (size_t j = 8; j < 20; j++)
        {
            sum += image[i * 28 + j];
        }
    }
    return sum / 144;
}

/*
// Function to read PGM image and store it into a 2D array
void readPGM(const char *filename, double* input, int *width, int *height) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read PGM header
    char magic[3];
    fscanf(file, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '5') {
        fprintf(stderr, "Not a PGM file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Read image dimensions
    fscanf(file, "%d %d", width, height);

    // Read maximum gray value
    int maxGray;
    fscanf(file, "%d", &maxGray);

    // Consume newline character after the max gray value
    fgetc(file);

    // Read pixel values into the array
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            input[i * *width + j] = fgetc(file);
        }
    }

    fclose(file);
}
*/
/*void createDataSet(char* foldername, double** inputs, double** refs, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        char *filename = malloc(20);
        sprintf(filename, "%s/%zu.pgm", foldername, i);

        inputs[i] = malloc(L1 * sizeof(double));
        int width, height;
        readPGM(filename, inputs[i], &width, &height);
        printf("%d x %d\n", width, height);
        refs[i] = malloc(L3 * sizeof(double));
        int n = i % 10;
        for (size_t j = 0; j < L3; j++)
            refs[i][j] = 0.0;
        refs[i][n] = 1.0;
    }
}*/

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(2, "invalid arguments : process [digitType] [network] [n]\n", 54);
    }
    network net;
    load(&net, argv[1]);

    /*double** inputs;
    double** refs;
    if (argv[1][0] == 't')
    {
        inputs = malloc(TRAININGS * sizeof(void*));
        refs = malloc(TRAININGS * sizeof(double*));

        createDataSet("typeScriptSet", inputs, refs, TRAININGS);
        shuffle(inputs, refs, TRAININGS);
    }*/

    FILE *file = fopen("testset/t10k-images.idx3-ubyte", "rb");
    if (file == NULL)
    {
        write(2, "can't open file\n", 16);
        return -1;
    }
    int magicN;
    int Nimages;
    int Nrow;
    int Ncol;
    fread(&magicN, sizeof(magicN), 1, file);
    fread(&Nimages, sizeof(Nimages), 1, file);
    fread(&Nrow, sizeof(Nrow), 1, file);
    fread(&Ncol, sizeof(Ncol), 1, file);
    magicN = ntohl(magicN);
    Nimages = ntohl(Nimages);
    Nrow = ntohl(Nrow);
    Ncol = ntohl(Ncol);

    printf("%d images %d x %d\n", Nimages, Nrow, Ncol);

    int Npixel = Nrow * Ncol;
    double **inputs = (double **)malloc(Nimages * sizeof(void *));
    for (int i = 0; i < Nimages; i++)
    {
        inputs[i] = (double *)malloc(Npixel * sizeof(double));
        for (int j = 0; j < Npixel; j++)
        {
            unsigned char tmp;
            fread(&tmp, sizeof(unsigned char), 1, file);
            inputs[i][j] = tmp;
        }
    }
    fclose(file);

    file = fopen("testset/t10k-labels.idx1-ubyte", "rb");
    magicN = 0;
    fread(&magicN, sizeof(magicN), 1, file);
    magicN = ntohl(magicN);
    int Nlabels;
    fread(&Nlabels, sizeof(Nlabels), 1, file);
    Nlabels = ntohl(Nlabels);
    double **refs = (double **)malloc(Nlabels * sizeof(void *));
    for (int i = 0; i < Nlabels; i++)
    {
        refs[i] = malloc(L3 * sizeof(double));
        unsigned char tmp;
        fread(&tmp, sizeof(unsigned char), 1, file);

        for (size_t j = 0; j < L3; j++)
        {
            refs[i][j] = (tmp == (unsigned char)j);
        }
    }
    fclose(file);
    printf("%d labels\n", Nlabels);

    test(&net, inputs, 30);

    return 0;
}
