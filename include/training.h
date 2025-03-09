void forwardPropagation(network *net, double *input);
int evaluate(network *net, double **inputs, double **refs, double threshold);
void backPropagation(network *net, double *input, double *ref, double rate);
void sgd(network *net, double **inputs, double **refs);
