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
//int M_SIZE;
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

int main()
{

}
