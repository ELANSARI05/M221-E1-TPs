#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size,tag = 100;
    MPI_Status status;
    int number ;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
            printf("Give the number: \n");
            fflush(stdout); 
            scanf("%d", &number);
            printf("I am process %d , i am the first process so i received the value %d from the user , and i will send it to process %d \n",rank,number,rank+1);
            MPI_Send(&number, 1, MPI_INT, rank+1,tag,MPI_COMM_WORLD);
    }
    else if(rank < size-1) {
        
        MPI_Recv(&number, 1, MPI_INT, rank-1,tag,MPI_COMM_WORLD,&status);
        number += rank;
        printf("I am process %d , i received the value %d from process %d , and i will send it (+ my rank) to process %d \n",rank,number,rank-1,rank+1);
        MPI_Send(&number, 1, MPI_INT, rank+1,tag,MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(&number, 1, MPI_INT, rank-1,tag,MPI_COMM_WORLD,&status);
        printf("I am process %d , i am the last process so i received the value %d from process %d , and i wont send to anyone \n",rank,number,rank-1);
    }
    

    MPI_Finalize();
    return 0;
}