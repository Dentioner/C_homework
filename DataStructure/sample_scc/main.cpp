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

//���룺5 5 AB BC CA DE ED
//�������һ����ͨ���� 34������021
//���룺5 3 AB BC DE
//�������һ����ͨ���� 3���� 4 ��0 �� 1 �� 2


