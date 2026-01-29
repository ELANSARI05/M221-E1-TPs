#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int t = 10;
    int n = 10;

    while (t--) {

        double a[n][n], b[n][n], c[n][n];

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

        clock_t t1 = clock();

        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++)
                for (int j = 0; j < n; j++)
                    c[i][j] += a[i][k] * b[k][j];

        clock_t t2 = clock();

        double time_taken = (double)(t2 - t1) / CLOCKS_PER_SEC;

        /* Memory bandwidth calculation */
        double bytes_accessed = 32.0 * n * n * n;   // 32 bytes per operation
        double bandwidth = bytes_accessed / time_taken / (1024.0 * 1024.0 * 1024.0);

        printf("n = %d | Time = %lf s | Bandwidth = %lf GB/s\n",
               n, time_taken, bandwidth);

        n += 50;
    }

    return 0;
}
