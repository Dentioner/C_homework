#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "MGraph.h"

using namespace std;

int main()
{
MGraph g;
CreateGraph(&g);

ListGraph(&g);
printf("MSTByPrim:\n");
MSTByPrim(&g,'A');

system("pause");
return 0;

}
//Input:
//7 11 0
//0 1 19 0 4 14 0 6 18 1 2 5 1 3 7 1 4 12 2 3 3 3 4 8 3 5 21 4 6 16 5 6 27
//Output:
/*
     A---1000  1910001000  141000  18
     B---  191000   5   7  1210001000
     C---1000   51000   3100010001000
     D---1000   7   31000   8  211000
     E---  14  121000   810001000  16
     F---100010001000  2110001000  27
     G---  18100010001000  16  271000
顶点编号=A 新增顶点=E weight=14
顶点编号=E 新增顶点=D weight=8
顶点编号=D 新增顶点=C weight=3
顶点编号=C 新增顶点=B weight=5
顶点编号=E 新增顶点=G weight=16
顶点编号=D 新增顶点=F weight=21
*/

