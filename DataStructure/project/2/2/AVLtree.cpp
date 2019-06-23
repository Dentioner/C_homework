#include "stdc++.h"
#define UNBALANCED 1
using namespace std;




struct picnode
{
	int flag, a;
	char c;
};

picnode g[1000][1000];
int max_line = 0, max_col = 0;


struct AVLnode
{
	int elem;
	AVLnode* lchild, *rchild;
	int height;

	AVLnode(int x, AVLnode *l, AVLnode *r, int h = 0) :
		elem(x), lchild(l), rchild(r), height(h) {}
};

int height(const AVLnode *);
void insert(const int, AVLnode * &);
void balance(AVLnode * &);
void rotateLchild(AVLnode * &);
void rotateRchild(AVLnode * &);
void doubleLchild(AVLnode * &);
void doubleRchild(AVLnode * &);
AVLnode * findmin(AVLnode *);
void deleteNode(int x, AVLnode * &);
void show(const AVLnode *);
void printTree(int space, const AVLnode *);
void merge(AVLnode* &, AVLnode* &);
void preOrder(const AVLnode *, vector<int> &);
void split(int x, AVLnode * &, AVLnode * &, AVLnode * &);
void destroy(AVLnode * &);
void showtree(int, AVLnode *);
int getleft(AVLnode *);
int getright(AVLnode *);



int height(const AVLnode * t)
{
	return t == NULL ? -1 : t->height;
}

void insert(const int x, AVLnode * & t)
{
	if (t == NULL)
		t = new AVLnode{ x,NULL,NULL };
	else if (x < t->elem)
		insert(x, t->lchild);
	else if (x > t->elem)
		insert(x, t->rchild);
	balance(t);
}

void balance(AVLnode * & t)
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

	t->height = max(height(t->lchild), height(t->rchild)) + 1;
}

void rotateLchild(AVLnode * & t)
{
	AVLnode * tmp = t->lchild;
	t->lchild = tmp->rchild;
	tmp->rchild = t;
	tmp->height = max(height(tmp->lchild), height(tmp->rchild)) + 1;
	t->height = max(height(t->lchild), height(t->rchild)) + 1;
	t = tmp;
}

void rotateRchild(AVLnode * & t)
{
	AVLnode * tmp = t->rchild;
	t->rchild = tmp->lchild;
	tmp->lchild = t;
	tmp->height = max(height(tmp->lchild), height(tmp->rchild)) + 1;
	t->height = max(height(t->lchild), height(t->rchild)) + 1;
	t = tmp;
}

void doubleLchild(AVLnode * & t)
{
	rotateRchild(t->lchild);
	rotateLchild(t);
}

void doubleRchild(AVLnode * & t)
{
	rotateLchild(t->rchild);
	rotateRchild(t);
}

AVLnode* findmin(AVLnode * t)
{
	AVLnode * i = t;
	while (i && i->lchild)
		i = i->lchild;
	return i;
}

void deleteNode(int x, AVLnode * & t)
{
	if (t == NULL)
		return;
	if (x < t->elem)
		deleteNode(x, t->lchild);
	else if (x > t->elem)
		deleteNode(x, t->rchild);
	else if (x == t->elem)
	{
		if (t->lchild && t->rchild)
		{
			AVLnode *tmp = findmin(t->rchild);
			t->elem = tmp->elem;
			deleteNode(tmp->elem, t->rchild);
		}

		else
		{
			AVLnode * save = t;
			t = (t->lchild) ? t->lchild : t->rchild;
			delete save;
		}
	}
	 
	balance(t);
}

void printTree(int space, const AVLnode * t)
{
	if (t)
	{
		printTree(space + 1, t->rchild);
		for (int i = 0; i < 5 * space; i++)
			printf("%c", ' ');
		printf("%5d\n\n", t->elem);
		printTree(space + 1, t->lchild);
	}
}

void preOrder(const AVLnode * t, vector<int> & a)
{
	if (t)
	{
		preOrder(t->lchild, a);
		a.push_back(t->elem);
		preOrder(t->rchild, a);
	}
}

void destroy(AVLnode * & t)
{
	if (t)
	{
		destroy(t->lchild);
		destroy(t->rchild);
		delete t;
	}
}

void merge(AVLnode * & t1, AVLnode * & t2)
{
	vector<int> a, b, c;
	preOrder(t1, a);
	preOrder(t2, b);
	int len1 = a.size(), len2 = b.size(), i = 0, j = 0;
	while (i < len1 && j < len2)
	{
		if (a[i] < b[j])
			c.push_back(a[i++]);
		else if (a[i] > b[j])
			c.push_back(b[j++]);
		else if (a[i] == b[j])
			i++;
	}
	while (i < len1)
		c.push_back(a[i++]);
	while (j < len2)
		c.push_back(b[j++]);
	t1 = NULL;
	for (i = 0; i < c.size(); i++)
		insert(c[i], t1);
	destroy(t2);
}

void split(int x, AVLnode * & t, AVLnode * & t1, AVLnode * & t2)
{
	vector<int> a;
	preOrder(t, a);
	int i = 0, len = a.size();
	for (; i < len; ++i)
		if (a[i] <= x && i + 1 < len && a[i + 1] > x)
			break;
	t1 = t2 = NULL;
	for (int j = 0; j <= i; ++j)
		insert(a[j], t1);

	for (int j = i + 1; j < len; ++j)
		insert(a[j], t2);
	destroy(t);
	return;
}

void show(const AVLnode * root)
{
	int h = height(root) + 1;
	if (!root)
	{
		cout << "An empty tree \n" << endl;
		return;
	}
	for (int i = 0; i < 10 * h; i++)
		cout << "*";
	cout << endl;
	printTree(0, root);
	for (int i = 0; i < 10 * h; i++)
		cout << "*";
	cout << endl;
}

int getleft(AVLnode * t)
{
	if (!t)
		return 0;
	else
	{
		//  if(!t -> lchild)
		  //    return 0;
		int l = getleft(t->lchild);
		int r = getright(t->lchild);
		return l + r + 1;
	}

}

int getright(AVLnode * t)
{
	if (!t)
		return 0;
	else
	{
		//if(!t -> rchild)
		  //  return 0;
		int l = getleft(t->rchild);
		int r = getright(t->rchild);
		return l + r + 1;
	}
}

void fillg(AVLnode * t, int x, int y)
{
	if (t)
	{
		g[x][y].a = t->elem;
		g[x][y].flag = 1;
		max_line = max(max_line, x);
		max_col = max(max_col, y);
		int r = getleft(t->rchild);
		int l = getright(t->lchild);
		int bound = max(l, r);
		if (l && r)
		{
			for (int i = 1; i <= bound; ++i)
			{
				g[x + i][y - i].flag = 0;
				g[x + i][y - i].c = '/';
			}
			for (int i = 1; i <= bound; ++i)
			{
				g[x + i][y + i].flag = 0;
				g[x + i][y + i].c = '\\';
			}
			fillg(t->lchild, x + bound + 1, y - bound - 1);
			fillg(t->rchild, x + bound + 1, y + bound + 1);
		}
		else
		{
			for (int i = 1; i <= l; ++i)
			{
				g[x + i][y - i].flag = 0;
				g[x + i][y - i].c = '/';
			}
			for (int i = 1; i <= r; ++i)
			{
				g[x + i][y + i].flag = 0;
				g[x + i][y + i].c = '\\';
			}
			fillg(t->lchild, x + l + 1, y - l - 1);
			fillg(t->rchild, x + r + 1, y + r + 1);
		}
	}
}

void showTree(AVLnode * t)
{
	for (int i = 0; i < 100; ++i)
		for (int j = 0; j < 100; ++j)
			g[i][j].flag = 0, g[i][j].c = ' ';
	max_line = 0, max_col = 0;
	int l = getright(t->lchild);
	int r = getleft(t->rchild);
	int bound = max(l, r);
	int maxl = getright(t);
	int maxr = getleft(t);
	int maxbound = max(maxl, maxr);
	fillg(t, 0, maxbound + 1);
	for (int i = 0; i <= max_line + 2; ++i)
		for (int j = 0; j < max_col + 2; ++j)
		{
			if (g[i][j].flag)
				printf("%d", g[i][j].a);
			else
				printf("%c", g[i][j].c);
			if (j == max_col + 1)
				printf("\n");
		}
}

int main()
{
	AVLnode* root = NULL;
	for (int i = 0; i < 10; ++i)
		insert(i, root);

	for (int i = 0; i < 100; ++i)
		for (int j = 0; j < 100; ++j)
			g[i][j].flag = 0, g[i][j].c = ' ';

	showTree(root);
	//deleteNode(7, root);
	for (int abc = 10; abc < 20; abc++)
		insert(abc, root);
	//insert(16, root);
	showTree(root);
	printTree(0, root);
	return 0;
}