double sigmoid(double z);
double magnitude(double *vect, size_t len);
double *feed(double **w, double *b, double *input, size_t n, size_t p);
double *vector_sum(double *v1, double *v2, size_t n);
double *vector_sub(double *v1, double *v2, size_t n);
void swap(double **a, double **b);
void shuffle(double **inputs, double **refs, size_t size);
