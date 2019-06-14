#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"

using namespace std;

int main()
{
	BiTree *t;
	t = CreateBiTree();
	printf("\nPreorder: "); PreorderTraverse(t, Visit);
	printf("\nInorder:"); InorderTraverse(t, Visit);
	printf("\nPostorder"); PostorderTraverse(t, Visit);
	system("Pause");
	return 0;
}
/*TestCase:
Input: ABZCDZZZEZFGHZZKZZZ;
Preorder Output:ABCDEFGHK
Inorder Output: BDCAEHGKF
Postorder Output:DCBHKGFEA
*/
