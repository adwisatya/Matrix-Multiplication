/* 
Created by: Aryya Dwisatya W
Date: 19 February 2015
Reference: http://stackoverflow.com/questions/7906825/dynamically-allocating-a-matrix-from-input-c
*/

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

int main(){
	FILE *fp;

	int **matrixA;
	int **matrixB;
	
	/* membuat dummy matrix */
	scanf("%d",&M_SIZE);
	printf("Matrix size %d ",M_SIZE);
	printf("\n");
	
	matrixA = CreateMatrix();
	matrixB = CreateMatrix();
	
	MatrixInput(matrixA);
	PrintMatrix(matrixA);
	printf("\n");

	/* perkalian dengan matrix kembar */
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
	PrintMatrix(matrixB);
}