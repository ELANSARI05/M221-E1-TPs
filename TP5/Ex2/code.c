#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int number = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    while (1) {
        if (rank == 0) {
            printf("Give the number: \n");
            fflush(stdout); 
            scanf("%d", &number);
        }
        MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (number < 0)
            break;
        printf("Process %d got %d\n", rank, number);
    }

    MPI_Finalize();
    return 0;
}