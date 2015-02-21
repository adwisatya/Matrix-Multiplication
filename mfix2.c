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

int **CreateMatrix(int M_SIZE){
	int **matrix = malloc(sizeof(int*)* M_SIZE);
	int row;
	for(row=0;row < M_SIZE;row++){
		matrix[row] = malloc(sizeof(int) * M_SIZE);
	}
	return matrix;
}
void MatrixInput(int ** matrix,int M_SIZE){
    int row, col;
    for(row=0;row<M_SIZE;row++){
            for (col=0; col<M_SIZE; col++){
                    scanf("%d",&matrix[row][col]);
            }
    }
}

void PrintMatrix(int ** matrix, int M_SIZE){
    int row,col;
    for (row=0;row<M_SIZE;row++){
            for(col = 0;col<M_SIZE;col++){
                    printf("%d ", matrix[row][col]);
            }
            printf("\n");
    }
}

int main(int argc, char *argv[]){
    int M_SIZE = 4;
    int ** matrixA;
    int ** matrixB;
    int ** matrixC;

    matrixC = CreateMatrix(M_SIZE);
    /* variabel untuk MPI */
    int myrank,P,from,to,i,j,k;
    int tag = 666;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Get_processor_name(processor_name,&name_len);

    if(myrank == 0){
        /* Jika master thread maka:
            - baca M_SIZE
            - broadcast M_SIZE
            - isi matrix
            - kirim matrix
        */
        scanf("%d",&M_SIZE);
        matrixA = CreateMatrix(M_SIZE);
        MatrixInput(matrixA,M_SIZE);
        matrixB = CreateMatrix(M_SIZE);
        MatrixInput(matrixB,M_SIZE);
        matrixC = CreateMatrix(M_SIZE);

        for(int x=1;x<M_SIZE;x++){
          printf("send to  %d with data from: %d and size:%d \n", x, (x)*M_SIZE/P, M_SIZE*M_SIZE/P);
          MPI_Send(&matrixA[x * M_SIZE / P][0], M_SIZE * M_SIZE / P, MPI_INT, x, 0, MPI_COMM_WORLD);
          MPI_Send(matrixB, M_SIZE * M_SIZE, MPI_INT, x, 0, MPI_COMM_WORLD);
        }
    }else{
        printf("Recv from %d with data from: %d and size:%d \n", 0, (myrank)*M_SIZE/P, M_SIZE*M_SIZE/P);
        MPI_Recv(&matrixA[myrank * M_SIZE / P][0], M_SIZE * M_SIZE / P, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
        MPI_Recv(matrixB, M_SIZE * M_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
    }

    /* Computation */
    for(i=myrank * M_SIZE / P; i<(myrank+1) * M_SIZE/P; ++i)
    {
      for (j=0; j < M_SIZE; j++)
      {
          matrixC[i][j] = 0;
          for (k=0; k<M_SIZE; k++)
          {
              matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
          }
      }
    }

    /* Result gathering */
    if (myrank != 0 )
    {
      MPI_Send(&matrixC[(myrank) * M_SIZE/P][0], M_SIZE*M_SIZE/P, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
      for (i=1; i<P; i++)
      {
          MPI_Recv(&matrixC[i * M_SIZE/P][0], M_SIZE * M_SIZE / P, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //MPI_Barrier(MPI_COMM_WORLD);
    if(myrank==0){
        PrintMatrix(matrixA,M_SIZE);
        printf("*\n");
        //PrintMatrix(matrixB);
        printf("=\n");
        PrintMatrix(matrixC,M_SIZE);
    }
    MPI_Finalize();
    return 0;
}
