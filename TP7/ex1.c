#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALIVE 1
#define DEAD  0

// Allocate 2D array
int** alloc_2d(int nx, int ny) {
    int** arr = malloc(nx * sizeof(int*));
    for (int i = 0; i < nx; i++)
        arr[i] = malloc(ny * sizeof(int));
    return arr;
}

// Initialize grid randomly
void init_grid(int** grid, int nx, int ny) {
    srand(time(NULL));
    for (int i = 1; i <= nx; i++)
        for (int j = 1; j <= ny; j++)
            grid[i][j] = rand() % 2;
}

// Count neighbors
int count_neighbors(int** grid, int i, int j) {
    int sum = 0;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
            if (dx != 0 || dy != 0)
                sum += grid[i+dx][j+dy];
    return sum;
}

// Copy grid
void copy_grid(int** src, int** dest, int nx, int ny) {
    for (int i = 1; i <= nx; i++)
        for (int j = 1; j <= ny; j++)
            dest[i][j] = src[i][j];
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Global grid size
    int NX = 16, NY = 16;
    int G = 10; // generations

    // Create 2D topology
    int dims[2] = {0, 0};
    MPI_Dims_create(size, 2, dims);

    int periods[2] = {1, 1}; // wrap-around
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    // Get coordinates
    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    // Find neighbors
    int north, south, west, east;
    MPI_Cart_shift(cart_comm, 0, 1, &north, &south);
    MPI_Cart_shift(cart_comm, 1, 1, &west, &east);

    // Local grid size
    int local_nx = NX / dims[0];
    int local_ny = NY / dims[1];

    // Allocate grids (with ghost cells)
    int** grid = alloc_2d(local_nx + 2, local_ny + 2);
    int** new_grid = alloc_2d(local_nx + 2, local_ny + 2);

    // Initialize
    init_grid(grid, local_nx, local_ny);

    // Initialize ghost cells (periodic boundary conditions)
    for (int i = 1; i <= local_nx; i++) {
        grid[i][0] = 0;
        grid[i][local_ny + 1] = 0;
    }
    for (int j = 0; j <= local_ny + 1; j++) {
        grid[0][j] = 0;
        grid[local_nx + 1][j] = 0;
    }

    // Buffers for column exchange
    int* send_left = malloc(local_nx * sizeof(int));
    int* send_right = malloc(local_nx * sizeof(int));
    int* recv_left = malloc(local_nx * sizeof(int));
    int* recv_right = malloc(local_nx * sizeof(int));

    for (int gen = 0; gen < G; gen++) {

        // === Exchange rows ===
        MPI_Sendrecv(&grid[1][0], local_ny + 2, MPI_INT, north, 0,
                     &grid[local_nx + 1][0], local_ny + 2, MPI_INT, south, 0,
                     cart_comm, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&grid[local_nx][0], local_ny + 2, MPI_INT, south, 1,
                     &grid[0][0], local_ny + 2, MPI_INT, north, 1,
                     cart_comm, MPI_STATUS_IGNORE);

        // === Prepare columns ===
        for (int i = 1; i <= local_nx; i++) {
            send_left[i-1]  = grid[i][1];
            send_right[i-1] = grid[i][local_ny];
        }

        // === Exchange columns ===
        MPI_Sendrecv(send_left, local_nx, MPI_INT, west, 2,
                     recv_right, local_nx, MPI_INT, east, 2,
                     cart_comm, MPI_STATUS_IGNORE);

        MPI_Sendrecv(send_right, local_nx, MPI_INT, east, 3,
                     recv_left, local_nx, MPI_INT, west, 3,
                     cart_comm, MPI_STATUS_IGNORE);

        // Copy received columns into ghost cells
        for (int i = 1; i <= local_nx; i++) {
            grid[i][0] = recv_left[i-1];
            grid[i][local_ny + 1] = recv_right[i-1];
        }

        // === Apply Game of Life ===
        for (int i = 1; i <= local_nx; i++) {
            for (int j = 1; j <= local_ny; j++) {
                int n = count_neighbors(grid, i, j);

                if (grid[i][j] == ALIVE)
                    new_grid[i][j] = (n == 2 || n == 3);
                else
                    new_grid[i][j] = (n == 3);
            }
        }

        // Copy new grid
        copy_grid(new_grid, grid, local_nx, local_ny);

        // Optional print
        if (rank == 0) {
            printf("Generation %d done\n", gen);
        }
    }

    // Final output (each process prints its block)
    for (int r = 0; r < size; r++) {
        MPI_Barrier(cart_comm);
        if (rank == r) {
            printf("Rank %d (coords [%d,%d]):\n", rank, coords[0], coords[1]);
            for (int i = 1; i <= local_nx; i++) {
                for (int j = 1; j <= local_ny; j++) {
                    printf("%d ", grid[i][j]);
                }
                printf("\n");
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
        free(grid[i]);
        free(new_grid[i]);
    }
    free(grid);
    free(new_grid);

    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}