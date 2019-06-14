#ifndef BTREE_H_INCLUDED
#pragma warning(disable:4996)
#include "stdio.h"
#define OK 1
#define ERROR 0
typedef int Status;
typedef char  TElemType;
typedef struct BiTNode {
	TElemType data;
	struct BiTNode *lchild, *rchild;
}BiTree;

BiTree *CreateBiTree();
Status Visit(TElemType e);
Status PreorderTraverse(BiTree *T, Status(*Visit)(TElemType e));
Status InorderTraverse(BiTree *T, Status(*Visit)(TElemType e));
Status PostorderTraverse(BiTree *T, Status(*Visit)(TElemType e));


#define BTREE_H_INCLUDED
#endif // BTREE_H_INCLUDED
