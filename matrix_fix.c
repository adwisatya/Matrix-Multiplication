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

int **CreateCustMatrix(int m, int n) {
	int **matrix = malloc(sizeof(int*)* m);
	int row;
	for(row=0;row < m;row++){
		matrix[row] = malloc(sizeof(int) * n);
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
    int M_SIZE;
    int ** matrixA;
    int ** matrixB;
    int ** matrixC;

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
            - kirim matrix */
        scanf("%d",&M_SIZE);
        matrixA = CreateMatrix(M_SIZE);
        MatrixInput(matrixA,M_SIZE);
        matrixB = CreateMatrix(M_SIZE);
        MatrixInput(matrixB,M_SIZE);
        //matrixC = CreateMatrix(M_SIZE);
    }
    MPI_Bcast(&M_SIZE,1, MPI_INT, 0, MPI_COMM_WORLD);
    //int from,to;
	int fromB,toB;
	int **tempMatrix;
    if(P<=M_SIZE)
    {
		from = myrank * M_SIZE/P;
		to = (myrank+1) * M_SIZE/P;
		MPI_Scatter(matrixA,M_SIZE*M_SIZE/P,MPI_INT,matrixA[from],M_SIZE*M_SIZE/P,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(matrixB,M_SIZE*M_SIZE,MPI_INT,0,MPI_COMM_WORLD);
	}
	else
	{
		from = myrank;
		to = (myrank+1);
		fromB = myrank * (M_SIZE/(P/M_SIZE));
		toB = (myrank+1) * (M_SIZE/(P/M_SIZE));
		MPI_Scatter(matrixA,M_SIZE,MPI_INT,matrixA[from],M_SIZE,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Scatter(matrixB,P,MPI_INT,matrixB[fromB],M_SIZE,MPI_INT,0,MPI_COMM_WORLD);
	}
	matrixC = CreateMatrix(M_SIZE);
	//tempMatrix = CreateCustMatrix(
	
	if(P<=M_SIZE)
	{
		for(i=from; i<to; i++)
		{
			for(j=0; j<M_SIZE; j++)
			{
				int sum = 0;
				for(k=0; k<M_SIZE; k++)
				{
					sum += matrixA[i][k] * matrixB[k][j];
					matrixC[i][j] = sum;
				}
			}
		}
	}
	else
	{
		for(i=from; i<to; i++)
		{
			for(j=0; j<M_SIZE; j++)
			{
				int sum = 0;
				for(k=fromB; k<toB; k++)
				{
					sum += matrixA[i][k] * matrixB[k][j];
					matrixC[i][j] = sum;
				}
			}
		}
	}
	
	if(P<=M_SIZE)
	{
		MPI_Gather(matrixC[from], M_SIZE*M_SIZE/P, MPI_INT, matrixC, M_SIZE*M_SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Gather(matrixC[from], M_SIZE/(P/M_SIZE), MPI_INT, matrixC, M_SIZE*M_SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
	}
	
	
    if(myrank == 0){
        //PrintMatrix(matrixA,M_SIZE);
        //printf("*\n");
        //PrintMatrix(matrixB);
        //printf("=\n");
        PrintMatrix(matrixC,M_SIZE);
    }
    MPI_Finalize();
    return 0;
}
