#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOL 1e-6
#define MAX_IT 10000

// Allocate 2D array
double** alloc_2d(int nx, int ny) {
    double** arr = malloc(nx * sizeof(double*));
    for (int i = 0; i < nx; i++)
        arr[i] = malloc(ny * sizeof(double));
    return arr;
}

// Initialize arrays
void init(double** u, double** f, int nx, int ny,
          int start_x, int start_y, double hx, double hy) {

    for (int i = 1; i <= nx; i++) {
        for (int j = 1; j <= ny; j++) {

            double x = (start_x + i - 1) * hx;
            double y = (start_y + j - 1) * hy;

            u[i][j] = 0.0;

            // f(x,y) = 2*(x^2 - x + y^2 - y)
            f[i][j] = 2.0 * (x*x - x + y*y - y);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Global grid
    int NX = 12, NY = 10;

    // Create topology
    int dims[2] = {0, 0};
    MPI_Dims_create(size, 2, dims);

    int periods[2] = {0, 0}; // NO wrap for Poisson
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    // Neighbors
    int north, south, west, east;
    MPI_Cart_shift(cart_comm, 0, 1, &north, &south);
    MPI_Cart_shift(cart_comm, 1, 1, &west, &east);

    // Local sizes
    int local_nx = NX / dims[0];
    int local_ny = NY / dims[1];

    // Global spacing
    double hx = 1.0 / (NX - 1);
    double hy = 1.0 / (NY - 1);

    // Allocate arrays (+2 for ghost)
    double** u = alloc_2d(local_nx + 2, local_ny + 2);
    double** u_new = alloc_2d(local_nx + 2, local_ny + 2);
    double** f = alloc_2d(local_nx + 2, local_ny + 2);

    // Starting indices in global grid
    int start_x = coords[0] * local_nx;
    int start_y = coords[1] * local_ny;

    init(u, f, local_nx, local_ny, start_x, start_y, hx, hy);

    // Initialize boundary conditions
    for (int j = 1; j <= local_ny; j++) {
        u[0][j] = 0.0;
        u[local_nx + 1][j] = 0.0;
    }
    for (int i = 1; i <= local_nx; i++) {
        u[i][0] = 0.0;
        u[i][local_ny + 1] = 0.0;
    }

    // Buffers for columns
    double* send_left = malloc(local_nx * sizeof(double));
    double* send_right = malloc(local_nx * sizeof(double));
    double* recv_left = malloc(local_nx * sizeof(double));
    double* recv_right = malloc(local_nx * sizeof(double));

    // Coefficients
    double coef1 = 0.5 * hx*hx * hy*hy / (hx*hx + hy*hy);
    double coef2 = 1.0 / (hx*hx);
    double coef3 = 1.0 / (hy*hy);

    double global_error = 1.0;
    int iter = 0;

    while (global_error > TOL && iter < MAX_IT) {

        // === Exchange rows ===
        MPI_Sendrecv(&u[1][0], local_ny+2, MPI_DOUBLE, north, 0,
                     &u[local_nx+1][0], local_ny+2, MPI_DOUBLE, south, 0,
                     cart_comm, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&u[local_nx][0], local_ny+2, MPI_DOUBLE, south, 1,
                     &u[0][0], local_ny+2, MPI_DOUBLE, north, 1,
                     cart_comm, MPI_STATUS_IGNORE);

        // === Columns ===
        for (int i = 1; i <= local_nx; i++) {
            send_left[i-1] = u[i][1];
            send_right[i-1] = u[i][local_ny];
        }

        MPI_Sendrecv(send_left, local_nx, MPI_DOUBLE, west, 2,
                     recv_right, local_nx, MPI_DOUBLE, east, 2,
                     cart_comm, MPI_STATUS_IGNORE);

        MPI_Sendrecv(send_right, local_nx, MPI_DOUBLE, east, 3,
                     recv_left, local_nx, MPI_DOUBLE, west, 3,
                     cart_comm, MPI_STATUS_IGNORE);

        for (int i = 1; i <= local_nx; i++) {
            u[i][0] = recv_left[i-1];
            u[i][local_ny+1] = recv_right[i-1];
        }

        // === Jacobi update ===
        double local_error = 0.0;

        for (int i = 1; i <= local_nx; i++) {
            for (int j = 1; j <= local_ny; j++) {

                u_new[i][j] = coef1 * (
                    coef2 * (u[i+1][j] + u[i-1][j]) +
                    coef3 * (u[i][j+1] + u[i][j-1]) -
                    f[i][j]
                );

                double diff = fabs(u_new[i][j] - u[i][j]);
                if (diff > local_error) local_error = diff;
            }
        }

        // Global error
        MPI_Allreduce(&local_error, &global_error, 1,
                      MPI_DOUBLE, MPI_MAX, cart_comm);

        // Copy
        for (int i = 1; i <= local_nx; i++)
            for (int j = 1; j <= local_ny; j++)
                u[i][j] = u_new[i][j];

        if (rank == 0 && iter % 100 == 0) {
            printf("Iteration %d global_error = %e\n", iter, global_error);
        }

        iter++;
    }

    if (rank == 0) {
        printf("Converged after %d iterations\n", iter);
    }

    // Print final solution (sample from rank 0)
    if (rank == 0) {
        printf("\nFinal solution u (rank 0):\n");
        for (int i = 1; i <= local_nx; i++) {
            for (int j = 1; j <= local_ny; j++) {
                printf("%.6f ", u[i][j]);
            }
            printf("\n");
        }
    }

    // Free memory
    free(send_left);
    free(send_right);
    free(recv_left);
    free(recv_right);

    for (int i = 0; i < local_nx + 2; i++) {
        free(u[i]);
        free(u_new[i]);
        free(f[i]);
    }
    free(u);
    free(u_new);
    free(f);

    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}