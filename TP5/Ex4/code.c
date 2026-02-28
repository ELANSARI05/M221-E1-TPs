#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void matrixVectorMult(double* A, double* b, double* x, int rows, int size) {
    for (int i = 0; i < rows; ++i) {
        x[i] = 0.0;
        for (int j = 0; j < size; ++j) {
            x[i] += A[i * size + j] * b[j];
        }
    }
}

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc != 2) {
        if (rank == 0)
            printf("Usage: %s <matrix_size>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int size = atoi(argv[1]);

    double *A = NULL, *b = NULL, *x_serial = NULL, *x_parallel = NULL;

    if (rank == 0) {
        A = malloc(size * size * sizeof(double));
        b = malloc(size * sizeof(double));
        x_serial = malloc(size * sizeof(double));
        x_parallel = malloc(size * sizeof(double));

        srand48(42);

        for (int i = 0; i < size * size; i++)
            A[i] = drand48();

        for (int i = 0; i < size; i++)
            b[i] = drand48();

        double t1 = MPI_Wtime();
        matrixVectorMult(A, b, x_serial, size, size);
        double t2 = MPI_Wtime();
        printf("Serial Time = %f seconds\n", t2 - t1);
    }

    if (rank != 0)
        b = malloc(size * sizeof(double));

    MPI_Bcast(b, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int base_rows = size / nprocs;
    int remainder = size % nprocs;
    int local_rows = base_rows + (rank < remainder ? 1 : 0);

    int *sendcounts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        sendcounts = malloc(nprocs * sizeof(int));
        displs = malloc(nprocs * sizeof(int));

        int offset = 0;
        for (int i = 0; i < nprocs; i++) {
            int rows = base_rows + (i < remainder ? 1 : 0);
            sendcounts[i] = rows * size;
            displs[i] = offset;
            offset += rows * size;
        }
    }

    double *local_A = malloc(local_rows * size * sizeof(double));

    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, local_rows * size, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    double *local_x = malloc(local_rows * sizeof(double));

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    matrixVectorMult(local_A, b, local_x, local_rows, size);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0)
        printf("Parallel Time = %f seconds\n", end - start);

    if (rank == 0) {
        int offset = 0;
        for (int i = 0; i < nprocs; i++) {
            int rows = base_rows + (i < remainder ? 1 : 0);
            sendcounts[i] = rows;
            displs[i] = offset;
            offset += rows;
        }
    }

    MPI_Gatherv(local_x, local_rows, MPI_DOUBLE,
                x_parallel, sendcounts, displs,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double max_error = 0.0;
        for (int i = 0; i < size; i++) {
            double diff = fabs(x_parallel[i] - x_serial[i]);
            if (diff > max_error)
                max_error = diff;
        }
        printf("Max error = %e\n", max_error);
    }

    MPI_Finalize();
    return 0;
}