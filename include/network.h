typedef struct network
{
    double **w2;
    double *b2;
    double *z2;
    double *a2;
    double **w3;
    double *b3;
    double *z3;
    double *a3;
} network;

void init_network(network *net);
void free_network(network *net);
void print_network(network *net);
network *network_copy(network *net);
