#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

int main() {
    float *a = malloc(N * sizeof(float));
    float sum = 0;
    double start, end;

    for (int i = 0; i < N; i++)
        a[i] = 1.0;

    int U = 1;
    while(U <= 32){
        start = (double)clock() / CLOCKS_PER_SEC;
        for (int i = 0; i < N; i++)
            sum += a[i];
        end = (double)clock() / CLOCKS_PER_SEC;

        printf("Sum = %f, Time = %f ms for Unrolling factor = %d \n", sum, (end - start) * 1000, U);
        U *= 2;
        sum = 0.0;
    }

    free(a);
    return 0;
}
