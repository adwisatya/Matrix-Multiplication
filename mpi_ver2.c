/*
Created by: Aryya Dwisatya W
Date: 19 February 2015
Reference:
http://stackoverflow.com/questions/7906825/dynamically-allocating-a-matrix-from-input-c
http://www.eecg.toronto.edu/~amza/ece1747h/homeworks/examples/MPI/other-examples/mmult.c
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
//#define M_SIZE 10
int M_SIZE=64;
void MatrixInput(int matrix[M_SIZE][M_SIZE]){
        int row, col;
        for(row=0;row<M_SIZE;row++){
                for (col=0; col<M_SIZE; col++){
                        scanf("%d",&matrix[row][col]);
                }
        }
}

void PrintMatrix(int matrix[M_SIZE][M_SIZE]){
        int row,col;
        for (row=0;row<M_SIZE;row++){
                for(col = 0;col<M_SIZE;col++){
                        printf("%d ", matrix[row][col]);
                }
                printf("\n");
        }
}

int main(int argc, char *argv[]){
        scanf("%d",&M_SIZE);
        printf("%d\n",M_SIZE);
        int matrixA[M_SIZE][M_SIZE], matrixB[M_SIZE][M_SIZE],matrixC[M_SIZE][M_SIZE];

        int myrank, P, from, to, i, j, k;
        int tag = 666;          /* any value will do */
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;

        MPI_Status status;
        MPI_Init (&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
        MPI_Comm_size(MPI_COMM_WORLD, &P); /* number of processors */
        MPI_Get_processor_name(processor_name,&name_len);

        if (M_SIZE%P!=0) {
                if (myrank==0) printf("Matrix size not divisible by number of processors\n");
                MPI_Finalize();
                exit(-1);
        }
        from = myrank * M_SIZE/P;
        to = (myrank+1) * M_SIZE/P;
        if(myrank == 0){
                /* Maka Master Thread */
                MatrixInput(matrixA);
                //MatrixInput(matrixB);
        }

        MPI_Bcast(matrixA,M_SIZE*M_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(matrixA,M_SIZE*M_SIZE/P,MPI_INT, matrixA[from],M_SIZE*M_SIZE/P,MPI_INT, 0,MPI_COMM_WORLD);

        printf("%s: computing slice %d (from row %d to %d)\n", processor_name, myrank, from, to-1);
        for (i=from; i<to; i++){
                for (j=0; j<M_SIZE; j++) {
                        matrixC[i][j]=0;
                        for (k=0; k<M_SIZE; k++){
                                matrixC[i][j] += matrixA[i][k]*matrixA[k][j];
                        }
                }
        }

        MPI_Gather (matrixC[from], M_SIZE*M_SIZE/P, MPI_INT, matrixC, M_SIZE*M_SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);

        //MPI_Barrier(MPI_COMM_WORLD);
        if(myrank==0){
                PrintMatrix(matrixA);
                printf("*\n");
                //PrintMatrix(matrixB);
                printf("=\n");
                PrintMatrix(matrixC);
        }
        MPI_Finalize();
        return 0;
}
