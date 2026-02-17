#include <stdio.h>
#include <omp.h>


static long num_steps = 100000000; // I changed this to bigger value so i the cpu time difference can be logic
double step;

int main () {
    double start =  omp_get_wtime();
    int i; double x, pi, sum = 0.0;
    step = 1.0 / (double) num_steps;
    
    #pragma omp parallel for
    
        for (i = 0; i < num_steps; i++) {
             x = (i + 0.5) * step;
             sum = sum + 4.0 / (1.0 + x * x);
        }
    
    
    
    pi = step * sum;
    printf("PI : %f \n",pi);
    double end =  omp_get_wtime();

    printf("Time : %f \n",end - start);

}