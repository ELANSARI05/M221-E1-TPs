#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define N 1000000

int main() {

    double *A = malloc(N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    double sum = 0.0;
    double mean = 0.0;
    double stddev = 0.0;
    double max1;

    // Initialization
    srand(0);
    for (int i = 0; i < N; i++)
        A[i] = (double)1;

    // Compute sum and max1
    sum = 0.0;
    max1 = 0.0;
    #pragma omp parallel 
    {
    #pragma omp sections
    {
    #pragma omp section 
    { 
        double temp_sum = 0.0;

        for (int i = 0; i < N; i++) {
            temp_sum += A[i];
        }
        #pragma omp atomic
        sum += temp_sum;
    }

    #pragma omp section
    {
        double temp_max1 = A[0];
        for (int i = 1; i < N; i++) {
            if (A[i] > temp_max1)
                temp_max1 = A[i];
        }
        #pragma omp atomic
        max1 += temp_max1;

    }
    }

    // Compute standard deviation
    #pragma omp single
    {

        mean = sum / N;

        double local_std = 0.0;
        for (int i = 0; i < N; i++)
            local_std += (A[i] - mean) * (A[i] - mean);

        stddev = sqrt(local_std / N);
    }
}


    // Print results
    printf("Sum     = %f\n", sum);
    printf("max1     = %f\n", max1);
    printf("Std Dev = %f\n", stddev);

    free(A);
    return 0;
}
