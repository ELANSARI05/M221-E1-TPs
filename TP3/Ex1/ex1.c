#include <stdio.h>
#include <omp.h>

int main(){

#pragma omp parallel num_threads(5)
{
    int rank;
    rank = omp_get_thread_num();
    printf("Hello from the rank %d thread \n", rank);
}
return 0 ;
}