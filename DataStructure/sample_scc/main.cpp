#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "OLGraph.h"
using namespace std;

int main()
{
OLGraph g;
CreateGraph(&g);
ConnectedComponent(&g);
system("pause");
return 0;
}

//输入：5 5 AB BC CA DE ED
//输出：第一个连通分量 34，二…021
//输入：5 3 AB BC DE
//输出：第一个连通分量 3，二 4 三0 四 1 五 2


