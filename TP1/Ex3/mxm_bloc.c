#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 50;
    double a[n][n], b[n][n], c[n][n];

    /* Initialization */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0.0;
            if (i == j)
                a[i][j] = 1.0;
            else
                a[i][j] = 0.0;
            b[i][j] = 1.0;
        }
    }

    /* Block size loop */
    for (int block = 1; block <= 50; block++) {

        /* Reset C */
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                c[i][j] = 0.0;

        clock_t t1 = clock();

        for (int i = 0; i < n; i += block)
            for (int j = 0; j < n; j += block)
                for (int k = 0; k < n; k += block)
                    for (int ii = i; ii < i + block && ii < n; ii++)
                        for (int jj = j; jj < j + block && jj < n; jj++)
                            for (int kk = k; kk < k + block && kk < n; kk++)
                                c[ii][jj] += a[ii][kk] * b[kk][jj];

        clock_t t2 = clock();

        /* CPU time */
        double cpu_time = (double)(t2 - t1) / CLOCKS_PER_SEC;

        /* Memory bandwidth calculation */
        double bytes_accessed = 32.0 * n * n * n;   // 32 bytes per inner iteration
        double bandwidth = bytes_accessed / cpu_time / (1024.0 * 1024.0 * 1024.0);

        printf("Block = %2d | Time = %lf s | Bandwidth = %lf GB/s\n",
               block, cpu_time, bandwidth);
    }

    return 0;
}
