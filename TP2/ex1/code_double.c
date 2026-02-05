#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

int main() {
    double *a = malloc(N * sizeof(double));
    double sum;
    double start, end;

    for (int i = 0; i < N; i++)
        a[i] = 1.0;

    /* U = 1 */
    sum = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 1)
        sum += a[i];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=1  Time=%f ms  Sum=%f\n", (end-start)*1000, sum);

    /* U = 2 */
    sum = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 2)
        sum += a[i] + a[i+1];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=2  Time=%f ms  Sum=%f\n", (end-start)*1000, sum);

    /* U = 4 */
    sum = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 4)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=4  Time=%f ms  Sum=%f\n", (end-start)*1000, sum);

    /* U = 8 */
    sum = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 8)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3]
             + a[i+4] + a[i+5] + a[i+6] + a[i+7];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=8  Time=%f ms  Sum=%f\n", (end-start)*1000, sum);

    /* U = 16 */
    sum = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 16)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3]
             + a[i+4] + a[i+5] + a[i+6] + a[i+7]
             + a[i+8] + a[i+9] + a[i+10] + a[i+11]
             + a[i+12] + a[i+13] + a[i+14] + a[i+15];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=16 Time=%f ms  Sum=%f\n", (end-start)*1000, sum);

    /* U = 32 */
    sum = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 32)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3]
             + a[i+4] + a[i+5] + a[i+6] + a[i+7]
             + a[i+8] + a[i+9] + a[i+10] + a[i+11]
             + a[i+12] + a[i+13] + a[i+14] + a[i+15]
             + a[i+16] + a[i+17] + a[i+18] + a[i+19]
             + a[i+20] + a[i+21] + a[i+22] + a[i+23]
             + a[i+24] + a[i+25] + a[i+26] + a[i+27]
             + a[i+28] + a[i+29] + a[i+30] + a[i+31];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=32 Time=%f ms  Sum=%f\n", (end-start)*1000, sum);

    free(a);
    return 0;
}
