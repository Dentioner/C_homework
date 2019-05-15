#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AGraph.h"
using namespace std;

int main()
{
AGraph g;int i,j;char c;
printf("Input the number of vertexes and graph kind(UDG-0,DG-1):");
scanf("%d%d",&i,&j); scanf("%c",&c);
CreateGraph(&g,i,j);
ListGraph(&g);

printf("BFS:");
BFS(&g);
system("pause");
return 0;
}
//Input the number of vertexes and graph kind:5 0
//AB AC AD BE CD DE XX
//BFS：A B C D E  ---广度遍历顺序

//Input the number of vertexes and graph kind:5 1
//AB AD CB CA CE DC ED XX
//BFS：A B D C E  ---广度遍历顺序

//Input the number of vertexes and graph kind:9 0
//AB AD AE DG EG GH HI BC CF XX
//BFS：A B D E C G F H I  ---广度遍历顺序
