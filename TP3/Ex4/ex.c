#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int m = 200, n = 200;

    double *a = (double *)malloc(m * n * sizeof(double));
    double *b = (double *)malloc(n * m * sizeof(double));
    double *c = (double *)malloc(m * m * sizeof(double));

    double start = omp_get_wtime();

    // Initialize matrices a and b
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = (i + 1) + (j + 1);
        }
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            b[i * m + j] = (i + 1) - (j + 1);
        }
    }

    // Initialize matrix c to zero
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            c[i * m + j] = 0;
        }
    }

    // Matrix multiplication (parallelized safely)
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < n; k++) {
                c[i * m + j] += a[i * n + k] * b[k * m + j];
            }
        }
    }

    double end = omp_get_wtime();

    printf("CPU Time : %f seconds\n", end - start);

    free(a);
    free(b);
    free(c);

    return 0;
}
