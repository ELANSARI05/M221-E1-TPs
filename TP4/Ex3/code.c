#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Light computation
void task_light(int N) {
    double x = 0.0;
    for (int i = 0; i < N; i++)
        x += sin(i * 0.001);
}

// Moderate computation
void task_moderate(int N) {
    double x = 0.0;
    for (int i = 0; i < 5*N; i++)
        x += sqrt(i * 0.5) * cos(i * 0.001);
}

// Heavy computation
void task_heavy(int N) {
    double x = 0.0;
    for (int i = 0; i < 20*N; i++)
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
}

int main() {
    int N = 100000;  // base workload

    double start_time, end_time;

    start_time = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        task_light(N);

        #pragma omp section
        task_moderate(N);

        #pragma omp section
        task_heavy(N);
    }

    end_time = omp_get_wtime();

    printf("Total execution time (parallel sections) = %lf seconds\n", end_time - start_time);

    return 0;
}
