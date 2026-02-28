#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, nprocs;
    long long int N = 1000000;
    double local_sum = 0.0, pi;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc > 1)
        N = atoll(argv[1]);

    long long int base = N / nprocs;
    long long int remainder = N % nprocs;
    long long int start = rank * base + (rank < remainder ? rank : remainder);
    long long int end   = start + base + (rank < remainder ? 1 : 0);

    for (long long int i = start; i < end; i++) {
        double x = (i + 0.5) / (double)N;
        local_sum += 4.0 / (1.0 + x*x);
    }

    MPI_Reduce(&local_sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi /= (double)N;
        printf("Approximated Pi = %.15f\n", pi);
    }

    MPI_Finalize();
    return 0;
}