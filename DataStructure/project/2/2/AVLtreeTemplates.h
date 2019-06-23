#pragma once
#include "stdc++.h"
#define UNBALANCED 1
template<typename ElemType>

struct AVLnode
{
	ElemType elem;
	AVLnode* lchild, *rchild;
	int height;

	AVLnode(ElemType x, AVLnode *l, AVLnode *r, int h = 0) :
		elem(x), lchild(l), rchild(r), height(h) {}
};

template<typename ElemType>
class AVLTree
{
	typedef struct AVLnode<ElemType> AVLnode;
public:
	AVLTree() :root(NULL) {}
	void insert(const ElemType);
	void deleteNode(ElemType x);
	void split(ElemType x, AVLTree<ElemType> &, AVLTree<ElemType> &);
	void merge(AVLTree &);
	void destroy(AVLnode * &);
	AVLnode * search(ElemType);
	void show();
	void destroy();
	void getarray(std::vector<ElemType> & a);
	ElemType top();


private:
	AVLnode *root;
private:
	int height(const AVLnode * t);
	void rotateLchild(AVLnode * &);
	void rotateRchild(AVLnode * &);
	void doubleLchild(AVLnode * &);
	void doubleRchild(AVLnode * &);
	void balance(AVLnode * &);
	void preOrder(const AVLnode *, std::vector<ElemType> &);
	void helpinsert(const ElemType x, AVLnode * & t);
	void helpdeleteNode(const ElemType x, AVLnode * & t);
	AVLnode* findmin(AVLnode *);
	void printTree(int space, const AVLnode *);
	void helpdestroy(AVLnode * &);

};

template<typename ElemType>
ElemType AVLTree<ElemType> ::top()
{
	return this->root->elem;
}


template<typename ElemType>
int AVLTree<ElemType> ::height(const AVLnode* t)
{
	return t == NULL ? -1 : t->height;
}

template<typename ElemType>
void AVLTree<ElemType> ::insert(ElemType x)
{
	helpinsert(x, this->root);
}

template<typename ElemType>
void AVLTree<ElemType> ::deleteNode(ElemType x)
{
	helpdeleteNode(x, this->root);
}

template<typename ElemType>
void AVLTree<ElemType> ::helpinsert(const ElemType x, AVLnode * & t)
{
	if (t == NULL)
		t = new AVLnode{ x,NULL,NULL };
	else if (x < t->elem)
		helpinsert(x, t->lchild);
	else if (x > t->elem)
		helpinsert(x, t->rchild);
	balance(t);
}

template<typename ElemType>
void AVLTree<ElemType> ::balance(AVLnode * & t)
{
	if (t == NULL)
		return;
	if (height(t->lchild) - height(t->rchild) > UNBALANCED)
		if (height(t->lchild->lchild) >= height(t->lchild->rchild))
			rotateLchild(t);
		else
			doubleLchild(t);

	else if (height(t->rchild) - height(t->lchild) > UNBALANCED)
		if (height(t->rchild->rchild) >= height(t->rchild->lchild))
			rotateRchild(t);
		else
			doubleRchild(t);

	t->height = std::max(height(t->lchild), height(t->rchild)) + 1;
}

template<typename ElemType>
void AVLTree<ElemType> ::rotateLchild(AVLnode * & t)
{
	AVLnode * tmp = t->lchild;
	t->lchild = tmp->rchild;
	tmp->rchild = t;
	tmp->height = std::max(height(tmp->lchild), height(tmp->rchild)) + 1;
	t->height = std::max(height(t->lchild), height(t->rchild)) + 1;
	t = tmp;
}

template<typename ElemType>
void AVLTree<ElemType> ::rotateRchild(AVLnode * & t)
{
	AVLnode * tmp = t->rchild;
	t->rchild = tmp->lchild;
	tmp->lchild = t;
	tmp->height = std::max(height(tmp->lchild), height(tmp->rchild)) + 1;
	t->height = std::max(height(t->lchild), height(t->rchild)) + 1;
	t = tmp;
}

template<typename ElemType>
void AVLTree<ElemType> ::doubleLchild(AVLnode * & t)
{
	rotateRchild(t->lchild);
	rotateLchild(t);
}

template<typename ElemType>
void AVLTree<ElemType> ::doubleRchild(AVLnode * & t)
{
	rotateLchild(t->rchild);
	rotateRchild(t);
}

template<typename ElemType>
AVLnode<ElemType>* AVLTree<ElemType> ::findmin(AVLnode * t)
{
	AVLnode * i = t;
	while (t && t->lchild)
		t = t->lchild;
	return t;
}

template<typename ElemType>
void AVLTree<ElemType> ::helpdeleteNode(const ElemType x, AVLnode * & t)
{
	if (t == NULL)
		return;
	if (x < t->elem)
		helpdeleteNode(x, t->lchild);
	else if (x > t->elem)
		helpdeleteNode(x, t->rchild);
	else if (x == t->elem)
	{
		if (t->lchild && t->rchild)
		{
			AVLnode *tmp = findmin(t->rchild);
			t->elem = tmp->elem;
			helpdeleteNode(tmp->elem, t->rchild);
		}

		else
		{
			AVLnode * save = t;
			t = (t->lchild) ? t->lchild : t->rchild;
			delete save;
		}
	}
}

template<typename ElemType>
void AVLTree<ElemType> ::show()
{
	printTree(0, this->root);
}

template<typename ElemType>
void AVLTree<ElemType> ::printTree(int space, const AVLnode * t)
{
	if (space == 0 && !t)
	{
		std::cout << "An empty Tree" << std::endl;
		return;
	}
	if (t)
	{
		printTree(space + 1, t->rchild);
		for (int i = 0; i < 5 * space; i++)
			printf("%c", ' ');
		printf("%5d\n\n", t->elem);
		printTree(space + 1, t->lchild);
	}
}

template<typename ElemType>
void AVLTree<ElemType> ::preOrder(const AVLnode * t, std::vector<ElemType> & a)
{
	if (t)
	{
		preOrder(t->lchild, a);
		a.push_back(t->elem);
		preOrder(t->rchild, a);
	}
}

template<typename ElemType>
void AVLTree<ElemType> ::split(ElemType x, AVLTree<ElemType> & t1, AVLTree<ElemType> & t2)
{
	std::vector<ElemType> a;
	preOrder(this->root, a);
	int i = 0, len = a.size();
	for (; i < len; ++i)
		if (a[i] <= x && i + 1 < len && a[i + 1] > x)
			break;

	for (int j = 0; j <= i; j++)
		t1.insert(a[j]);
	for (int j = i + 1; j < len; j++)
		t2.insert(a[j]);
}

template<typename ElemType>
void AVLTree<ElemType> ::destroy()
{
	helpdestroy(this->root);
}

template<typename ElemType>
void AVLTree<ElemType> ::helpdestroy(AVLnode * & t)
{
	if (t)
	{
		helpdestroy(t->lchild);
		helpdestroy(t->rchild);
		delete(t);
	}
}

template<typename ElemType>
void AVLTree<ElemType> ::getarray(std::vector<ElemType> & a)
{
	preOrder(this->root, a);
}

template<typename ElemType>
void AVLTree<ElemType> ::merge(AVLTree & t)
{
	std::vector<ElemType> a, b;
	getarray(a);
	t.getarray(b);
	int len1 = a.size(), len2 = b.size(), i = 0, j = 0;
	helpdestroy(this->root);
	while (i < len1 && j < len2)
	{
		if (a[i] < b[j])
			helpinsert(a[i++], this->root);
		else if (a[i] > b[j])
			helpinsert(b[j++], this->root);
		else if (a[i] == b[j])
			i++;
	}

	while (i < len1)
		helpinsert(a[i++], this->root);
	while (j < len2)
		helpinsert(b[j++], this->root);
	return;
}