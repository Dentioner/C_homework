#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<time.h>
#define m 5
#define p 5
#define n 5

int main(int argc, char** argv)
{
    int *A, *B, *C;
    int i, j ,k;
    int ID, num_procs, line;
    MPI_Status status;
    srand(time(0));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    A = (int *)malloc(sizeof(int)*m*p);
    B = (int *)malloc(sizeof(int)*p*n);
    C = (int *)malloc(sizeof(int)*m*n);
    line = n/num_procs;

    if(ID == 0)
    {
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < p; j++)
            {
                A[i*m+j] = rand()%20;
            }
        }

        for (i = 0; i < p; i++)
        {
            for (j = 0; j < n; j++)
            {
                B[i*p+j] = rand()%20;
            }
        }

        for(i = 1; i < num_procs; i++)
        {
            MPI_Send(B, p*n, MPI_INT, i ,0, MPI_COMM_WORLD);
            MPI_Send(A + (i-1)*line*m, line*p, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        for(i = 1; i <num_procs;i++)
        {
            MPI_Recv(C + (i-1)*line*m, line*n, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }

        for(i = (num_procs - 1)*line; i < m; i++)
        {
            for(j = 0; j < n; j++)
            {
                C[i*m + j] = 0;
                for(k = 0; k < p; k++)
                {
                    C[i*m + j] += A[i*m + k]*B[k*p + j];
                }
            }
        }
    }

    else
    {
        MPI_Recv(B, p*n, MPI_INT, 0,0, MPI_COMM_WORLD, &status);
        MPI_Recv(A + (ID-1)*line*m, line*p, MPI_INT, 0,1, MPI_COMM_WORLD, &status);

        for(i = (ID-1)*line; i < ID*line; i++)
        {
            for(j = 0; j < n; j++)
            {
                C[i*m+j] = 0;
                for(k = 0; k < p; k++)
                {
                    C[i*m + j] += A[i*m+k]*B[k*p+j];
                }
                MPI_Send(C+(num_procs - 1)*line*m, line*n, MPI_INT, 0, 2, MPI_COMM_WORLD);
            }
        }
    }
    
    
    MPI_Finalize();

        printf("A:\n");
	int index1, index2;
    for (index1 = 0, index2 = 0; index1 < m*p; index1++, index2++)
	{
			
		printf("%d\t", A[index1]);
		if(index2 == p - 1)
		{
			printf("\n");
			index2 = -1;
		}
	}

    printf("B:\n");
    for (index1 = 0, index2 = 0; index1 < p*n; index1++, index2++)
	{
			
		printf("%d\t", B[index1]);
		if(index2 == n - 1)
		{
			printf("\n");
			index2 = -1;
		}
	}

    printf("C:\n");
	for (index1 = 0, index2 = 0; index1 < m*n; index1++, index2++)
	{
			
		printf("%d\t", C[index1]);
		if(index2 == n - 1)	
		{
			printf("\n");
			index2 = -1;
		}
	}
    
    return 0;
}
