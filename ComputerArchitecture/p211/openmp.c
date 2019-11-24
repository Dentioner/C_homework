#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>
#define m 3
#define p 4
#define n 5

int A[m][p], B[p][n], C[m][n];

int main()
{
	int i, j, k;
	srand(time(0));
    for (i = 0; i < m; i++)
	{
		for (j = 0; j < p; j++)
		{
			A[i][j] = rand()%20;
		}
	}

    for (i = 0; i < p; i++)
	{
		for (j = 0; j < n; j++)
		{
			B[i][j] = rand()%20;
		}
	}
#pragma omp parallel for shared(A, B, C) private(i, j, k)
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			C[i][j] = 0;
			for (k = 0; k < p; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

    printf("A:\n");
    for (int index1 = 0; index1 < m; index1++)
	{
		for (int index2 = 0; index2 < p; index2++)
		{
			
			printf("%d\t", A[index1][index2]);
			
		}
		printf("\n");
		
	}

    printf("B:\n");
    for (int index1 = 0; index1 < p; index1++)
	{
		for (int index2 = 0; index2 < n; index2++)
		{
			
			printf("%d\t", B[index1][index2]);
			
		}
		printf("\n");
		
	}

    printf("C:\n");
	for (int index1 = 0; index1 < m; index1++)
	{
		for (int index2 = 0; index2 < n; index2++)
		{
			
			printf("%d\t", C[index1][index2]);
			
		}
		printf("\n");
		
	}
	return 0;
}