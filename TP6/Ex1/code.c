#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    MPI_Datatype column;

    int a[4][5];
    int b[5][4];

    MPI_Type_vector(4, 1, 5, MPI_INT, &column);
    MPI_Type_commit(&column);

    if(rank == 0){
        printf("Process 0 - Matrix a: \n");
        for(int i=0;i<4;i++){
            for(int k=0;k<5;k++){
                a[i][k] = i*5 + k + 1;
                printf("%d ",a[i][k]);
            }
            printf("\n");
        }

        printf("\n");

        for(int i=0;i<5;i++){
            MPI_Send(&a[0][i], 1, column, 1, i, MPI_COMM_WORLD);
        }
    }

    else if(rank == 1){

        for(int i=0;i<5;i++){
            MPI_Recv(b[i], 4, MPI_INT, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }


        printf("Process 1 - Matrix transposee at: \n");
        for(int i=0;i<5;i++){
            for(int j=0;j<4;j++){
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}