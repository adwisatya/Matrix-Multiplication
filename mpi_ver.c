/* 
Created by: Aryya Dwisatya W
Date: 19 February 2015
Reference: http://stackoverflow.com/questions/7906825/dynamically-allocating-a-matrix-from-input-c
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int M_SIZE;
int **CreateMatrix(){
	int **matrix = malloc(sizeof(int*)* M_SIZE);
	int row;
	for(row=0;row < M_SIZE;row++){
		matrix[row] = malloc(sizeof(int) * M_SIZE);
	}
	return matrix;
}

void MatrixInput(int **matrix){
	int row, col;
	for(row=0;row<M_SIZE;row++){
		for (col=0; col<M_SIZE; col++){
			scanf("%d",&matrix[row][col]);
		}
	}
}

void PrintMatrix(int **matrix){
	int row,col;
	for (row=0;row<M_SIZE;row++){
		for(col = 0;col<M_SIZE;col++){
			printf("%d ", matrix[row][col]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[]){

	int **matrixA;
	int **matrixB;
	int myrank, P, from, to,y;
	int tag = 666;

	
	MPI_Status status;
  
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* who am i */
	MPI_Comm_size(MPI_COMM_WORLD, &P); /* number of processors */

	if (M_SIZE%P!=0) {
		if (myrank==0) printf("Matrix size not divisible by number of processors\n");
		MPI_Finalize();
		exit(-1);
	}

	from = myrank * M_SIZE/P;
	to = (myrank+1) * M_SIZE/P;
	
	if (myrank==0) {
	/* membuat dummy matrix */
	scanf("%d",&M_SIZE);
	printf("Matrix size %d ",M_SIZE);
	printf("\n");
	
	matrixA = CreateMatrix();
	matrixB = CreateMatrix();
	
	MatrixInput(matrixA);
	PrintMatrix(matrixA);
	printf("\n");
		matrixA = CreateMatrix();
		matrixB = CreateMatrix();
	}
	
	MPI_Bcast (matrixA, M_SIZE*M_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter (matrixA, M_SIZE*M_SIZE/P, MPI_INT, matrixA[from], M_SIZE*M_SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);

	printf("computing slice %d (from row %d to %d)\n", myrank, from, to-1);
	/* perkalian dengan matrix kembar */
	for (y=from; y<to; y++){
		for(int j=0;j<M_SIZE;j++){
			for(int i=0;i<M_SIZE;i++){
				matrixB[i][j] =  0;
			}
			for(int k=0;k<M_SIZE;k++){
				for(int i=0;i<M_SIZE;i++){
					matrixB[i][j] = matrixB[i][j] + matrixA[i][k] * matrixA[k][j];
				}
			}
		}
	}
	MPI_Gather (matrixB[from], M_SIZE*M_SIZE/P, MPI_INT, matrixB, M_SIZE*M_SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
	if (myrank==0) {
		printf("\n\n");
		PrintMatrix(matrixB);
		printf("\n\n\t       * \n");
	}

	MPI_Finalize();
	return 0;
  
}