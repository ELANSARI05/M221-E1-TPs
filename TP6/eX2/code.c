#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#define N_FEATURES 5
#define N_SAMPLES 1000
#define LR 0.001
#define MAX_EPOCHS 1000
#define TH 1e-2

typedef struct{
    double x[N_FEATURES];
    double y;
} Sample;

void generate_data(Sample *d,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<N_FEATURES;j++)
            d[i].x[j] = (double)rand()/RAND_MAX;
        d[i].y = 2*d[i].x[0] - d[i].x[1];
    }
}

int main(int argc,char **argv){

    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    MPI_Datatype MPI_SAMPLE;

    int block[2]={N_FEATURES,1};
    MPI_Aint disp[2];
    MPI_Datatype types[2]={MPI_DOUBLE,MPI_DOUBLE};

    disp[0]=offsetof(Sample,x);
    disp[1]=offsetof(Sample,y);

    MPI_Type_create_struct(2,block,disp,types,&MPI_SAMPLE);
    MPI_Type_commit(&MPI_SAMPLE);

    Sample *data=NULL;

    if(rank==0){
        data=malloc(N_SAMPLES*sizeof(Sample));
        generate_data(data,N_SAMPLES);
    }

    int local_n=N_SAMPLES/size;
    Sample *local=malloc(local_n*sizeof(Sample));

    int *counts=NULL,*displs=NULL;

    if(rank==0){
        counts=malloc(size*sizeof(int));
        displs=malloc(size*sizeof(int));
        for(int i=0;i<size;i++){
            counts[i]=local_n;
            displs[i]=i*local_n;
        }
    }

    MPI_Scatterv(data,counts,displs,MPI_SAMPLE,
                 local,local_n,MPI_SAMPLE,
                 0,MPI_COMM_WORLD);

    double w[N_FEATURES]={0};

    double start=MPI_Wtime();

    for(int e=1;e<=MAX_EPOCHS;e++){

        double grad[N_FEATURES]={0};
        double loss=0;

        for(int i=0;i<local_n;i++){

            double pred=0;

            for(int j=0;j<N_FEATURES;j++)
                pred+=w[j]*local[i].x[j];

            double err=pred-local[i].y;

            loss+=err*err;

            for(int j=0;j<N_FEATURES;j++)
                grad[j]+=err*local[i].x[j];
        }

        MPI_Allreduce(MPI_IN_PLACE,grad,N_FEATURES,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
        MPI_Allreduce(MPI_IN_PLACE,&loss,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

        double mse=loss/N_SAMPLES;

        for(int j=0;j<N_FEATURES;j++)
            w[j]-=LR*grad[j]/N_SAMPLES;

        if(rank==0 && e%10==0)
            printf("Epoch %d | Loss %f | w0 %f w1 %f\n",e,mse,w[0],w[1]);

        if(mse<TH){
            if(rank==0)
                printf("Stop at epoch %d\n",e);
            break;
        }
    }

    double end=MPI_Wtime();

    if(rank==0)
        printf("Time %f\n",end-start);

    MPI_Finalize();
}