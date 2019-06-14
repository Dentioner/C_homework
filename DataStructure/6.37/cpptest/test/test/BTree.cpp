
#include <malloc.h>
#include "BTree.h"
//#include "SqStack.h"

BiTree *CreateBiTree() {
	//����������������н���ֵ���ַ����ո��Z��ʾ������������������ʾ�Ķ�����

	BiTree *bt;
	TElemType x;
	scanf("%c", &x);
	if (x == 'Z') bt = NULL;
	else {
		bt = (BiTree *)malloc(sizeof(BiTree));
		if (!bt) return NULL;
		bt->data = x;
		bt->lchild = CreateBiTree();
		bt->rchild = CreateBiTree();
	}
	return bt;
}

Status Visit(TElemType e) {
	printf(" %c ", e);
	return OK;

}

Status PreorderTraverse(BiTree *T, Status(*Visit)(TElemType e))
{
	if (T) {
		Visit(T->data);
		PreorderTraverse(T->lchild, Visit);
		PreorderTraverse(T->rchild, Visit);
	}
	return OK;
	/*if (T) {
			if (Visit(T->data))            // ���ʽ��
			 if (PreorderTraverse (T->lchild, Visit)) //����������
				if (PreorderTraverse (T->rchild, Visit)) //����������
				return OK;
			return ERROR;
		}
		else  return OK;
	*/
}

Status InorderTraverse(BiTree *T, Status(*Visit)(TElemType e))
{
	if (T) {
		InorderTraverse(T->lchild, Visit);
		Visit(T->data);
		InorderTraverse(T->rchild, Visit);
	}
	return OK;
	/*
		if (T) {
			if(InorderTraverse (T->lchild, Visit))//����������
				if(Visit(T->data))
					if(InorderTraverse (T->rchild, Visit))//����������
						return OK;
			return ERROR;
		}
		else  return OK;
	*/
}

Status PostorderTraverse(BiTree *T, Status(*Visit)(TElemType e))
{
	if (T)
	{
		if (PostorderTraverse(T->lchild, Visit))//����������
			if (PostorderTraverse(T->rchild, Visit))//����������
				if (Visit(T->data))
					return OK;
		return ERROR;
	}
	else  return OK;
}
