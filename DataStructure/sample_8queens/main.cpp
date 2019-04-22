#include <iostream>
#include "stdio.h"
#include "stdlib.h"
using namespace std;

#define CHESSBOARD_SIZE 4
int count;

void Queens(int chess[],int i){
int j,k;
if(i==CHESSBOARD_SIZE){
    for(j=0;j<CHESSBOARD_SIZE;j++)
        printf("%d,",chess[j]+1);
    printf("\n");
    count++;
    return;
   }
for(j=0;j<CHESSBOARD_SIZE;j++){
    chess[i]=j;
    for(k=0; k<i && (chess[i]!=chess[k] && i-chess[i] !=k-chess[k] && i+chess[i]!=k+chess[k]);k++);
    if (k==i)
        Queens(chess,i+1);
    else
        chess[i]= -1;
    }
}
//4*4ÆåÅÌ£º2,4,1,3£»3,1,4,2
int main()
{
int chess[CHESSBOARD_SIZE],i,j;
count =0;
for(i=0;i<CHESSBOARD_SIZE;i++){
    for(j=0;j<CHESSBOARD_SIZE;j++) chess[j]= -1;
    chess[0]=i;
    Queens(chess,1);
   }
printf("Valid layouts£º%d\n",count);
system("Pause");
return 0;
}
