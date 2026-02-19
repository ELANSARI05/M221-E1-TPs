#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void dmvm_v1(int n, int m, double *lhs, double *rhs, double *mat) {
    /* simple parallel-for (parallel-for has implicit barrier at end) */
    #pragma omp parallel for
    for (int r = 0; r < m; ++r) {
        double sum = 0.0;
        for (int c = 0; c < n; ++c)
            sum += mat[r + c*m] * rhs[c];
        lhs[r] = sum;
    }
}

void dmvm_v2(int n, int m, double *lhs, double *rhs, double *mat) {
    /* explicit parallel region, for with schedule(dynamic) and nowait */
    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic) nowait
        for (int r = 0; r < m; ++r) {
            double sum = 0.0;
            for (int c = 0; c < n; ++c)
                sum += mat[r + c*m] * rhs[c];
            lhs[r] = sum;
        }
        /* If you had additional work that can overlap with other threads' loops,
           you would put it here. The parallel region end will still act as a
           global join before the function returns. */
    }
}

void dmvm_v3(int n, int m, double *lhs, double *rhs, double *mat) {
    /* explicit parallel region, for with schedule(static) and nowait */
    #pragma omp parallel
    {
        #pragma omp for schedule(static) nowait
        for (int r = 0; r < m; ++r) {
            double sum = 0.0;
            for (int c = 0; c < n; ++c)
                sum += mat[r + c*m] * rhs[c];
            lhs[r] = sum;
        }
    }
}

int main(void) {
    const int n = 40000; // columns
    const int m = 600;   // rows

    double *mat = malloc((size_t)n * m * sizeof(double));
    double *rhs = malloc((size_t)n * sizeof(double));
    double *lhs = malloc((size_t)m * sizeof(double));

    if (!mat || !rhs || !lhs) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    /* initialization */
    for (int c = 0; c < n; ++c) {
        rhs[c] = 1.0;
        for (int r = 0; r < m; ++r)
            mat[r + c*m] = 1.0;
    }

    double start, end;

    /* Version 1 */
    for (int r = 0; r < m; ++r) lhs[r] = 0.0;
    start = omp_get_wtime();
    dmvm_v1(n, m, lhs, rhs, mat);
    end = omp_get_wtime();
    printf("V1 CPU time = %lf seconds\n", end - start);

    /* Version 2 */
    for (int r = 0; r < m; ++r) lhs[r] = 0.0;
    start = omp_get_wtime();
    dmvm_v2(n, m, lhs, rhs, mat);
    end = omp_get_wtime();
    printf("V2 CPU time = %lf seconds\n", end - start);

    /* Version 3 */
    for (int r = 0; r < m; ++r) lhs[r] = 0.0;
    start = omp_get_wtime();
    dmvm_v3(n, m, lhs, rhs, mat);
    end = omp_get_wtime();
    printf("V3 CPU time = %lf seconds\n", end - start);

    free(mat);
    free(rhs);
    free(lhs);
    return 0;
}
