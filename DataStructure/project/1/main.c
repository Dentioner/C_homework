#include<stdio.h>
#include<stdlib.h>


int board[8][8];
int location[2];
int step_counter;
int vector[8][2] = {{-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}};

void printboard()
{
    for (int i = 0; i<8;i++)
    {
        for (int j  = 0; j<8;j++)
        {
            printf("%-4d", board[i][j]);
        }
        printf("\n");
    }
}

void next_step()
{
    
}

int main()
{
    printboard();
    system("pause");
    return 0;
}

