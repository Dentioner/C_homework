#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10
typedef int elemtype;

typedef struct BiTNode
{
	elemtype data;
	struct BiTNode *lchild, *rchild, *parent;
}node, *tree;

typedef struct
{
	int top; //栈顶
	tree *base;
	int stacksize;
}SqStack; //动态顺序栈

int data_array[MAXLENGTH];
tree node_array[MAXLENGTH];//这个用来存储每个节点的指针，方便找到它
int visit_array[MAXLENGTH];
int length = 0;
int length_notNULL = 0;//记录非0节点个数
int visit_counter = 0;
int a, b;
int findit = 0;
//比如说我想找编号为12的节点，直接在这个地方找nodearray[12]就行了
//nodearray[0]存放长度

void create_array()
{
	int ch;
	int number = -1;
	int index = 0;
	scanf("%d", &number);
	ch = getchar();
	while (ch != EOF && ch != '\n' && ch != '\0' && ch!=';')
	{

		if (number == -1)//null
		{
			//ch = getchar();//n
			ch = getchar();//u
			ch = getchar();//l
			ch = getchar();//l

			//注意这个地方没有考虑最后一个输入为null的情况
			//如果最后一个输入为null，那么下一个字符就不是逗号而是回车
			ch = getchar();//,
			if (ch == '\n')
				break;


			data_array[index] = -1;//用-1表示null        
		}
		else
		{
			data_array[index] = number;
			length_notNULL++;
		}


		//下一个
		number = -1;
		scanf("%d", &number);
		ch = getchar();
		index++;
	}


	if (number == -1)//null
	{
		//ch = getchar();//n
		//ch = getchar();//u
	   // ch = getchar();//l
	   // ch = getchar();//l

		data_array[index++] = -1;//用-1表示null        
	}
	else
	{
		data_array[index++] = number;
		length_notNULL++;
	}

	data_array[index] = '\0';

	length = index;

	scanf("%d;%d", &a, &b);//找祖先的两个点
	


	return;


}

tree root;

void create_tree_array()
{
	int index = 0;
	tree  p;
	root = (tree)malloc(sizeof(node));
	root->lchild = root->rchild = root->parent = NULL;

	node_array[0] = root;
	p = root;

	//while (data_array[index] != '\0')
	while(index < length)
	{
		if (data_array[index] != -1)
			p->data = data_array[index];
		else
			p = NULL;
		node_array[index + 1] = p;

		p = (tree)malloc(sizeof(node));
		p->lchild = p->rchild = p->parent = NULL;
		p->data = -1;
		index++;
	}
	node_array[index + 1] = NULL;



	return;
}

void move(int end_index)
{
	int start = length;
	for (; start >= end_index; start--)
	{
		node_array[start + 2] = node_array[start];
		node_array[start] = NULL;
	}
	length += 2;
	return;
}

void rearrangement()//由于输入的数据不完全是按照层次遍历完全二叉树的那种编号来的
{//因此需要将数组整理为完全二叉树的那种样子
//比如说像这个数组1,2,3,null,null,null,4,null,5
//由于2的下家全是null，实际上这里省去了好多个null
//5实际上应该出现在第15个位置才对
	tree n, lchild, rchild;
	int index = 1;
	int counter = 0;
	int lcindex;
	int rcindex;
	while (counter < length_notNULL)
	{
		lcindex = 2 * index;
		rcindex = 2 * index + 1;
		n = node_array[index];
		lchild = node_array[lcindex];
		rchild = node_array[rcindex];
		if (n == NULL)
		{//把后面的节点往后挪
			move(index * 2);
		}
		else
		{
			counter++;
		}
		index++;
	}

	return;
}


void create_tree_relationship()
{
	tree n, before;
	int index = 1;

	n = before = node_array[index];
	while (index <= length)
	{

		n = node_array[index];
		if (n != NULL)
		{
			n->lchild = node_array[2 * index];
			n->rchild = node_array[2 * index + 1];
			if (n->lchild != NULL)
				n->lchild->parent = n;
			if (n->rchild != NULL)
				n->rchild->parent = n;

		}
		index++;



	}

	return;
}

void Visit(elemtype e)
{
	/*
	if (visit_counter < length - 1)
		printf("%d,", e);
	else
		printf("%d", e);
	visit_counter++;
	*/
	if (e == b)
		findit = 1;
	visit_array[visit_counter] = e;
	visit_array[visit_counter + 1] = '\0';
	visit_counter++;
	return;
}

void InitStack(SqStack *s)
{
	s->base = (tree*)malloc(MAXLENGTH * sizeof(tree));
	if (!s)
		exit(1);
	s->top = 0;
	s->stacksize = length;
	return;
}

int GetLen(SqStack *s)
{
	return (s->top);
}



void Push(SqStack *s, tree e)
{
	if (s->top >= s->stacksize)
	{ //栈满
		s->base = (tree *)realloc(s->base, (s->stacksize + ADDLENGTH) * sizeof(tree));
		if (!s->base)
			exit(1);
		s->stacksize += ADDLENGTH;
	}
	s->base[s->top++] = e;
	return;
}

void Pop(SqStack *s, tree *e)
{
	if (s->top == 0)
		exit(1);
	*e = s->base[--s->top];
	return;
}

int IsStackEmpty(SqStack *s)
{
	if (s->top == 0)
		return 1;
	else
		return 0;
}

void PreorderTraverse(tree T, void(*Visit)(elemtype e))
{
	if (T) {
		Visit(T->data);
		PreorderTraverse(T->rchild, Visit);
		PreorderTraverse(T->lchild, Visit);
	}
	return;
}


void PreorderTraverse_no_recur(void(*Visit)(elemtype e))
{
	SqStack s;
	InitStack(&s);
	tree p;
	//elemtype visit_data;
	p = root;
	while (p != NULL || !IsStackEmpty(&s))//同时满足才说明结束了
	{
		if (p != NULL)
		{
			Visit(p->data);
			Push(&s, p);

			p = p->lchild;
		}
		else
		{


			Pop(&s, &p);

			p = p->rchild;
		}
	}
	return;
}

void visit_print()
{
	int index;
	for (index = 0; index < visit_counter - 1; index++)
		printf("%d,", visit_array[index]);
	printf("%d", visit_array[index]);
	return;
}



void find_ancestor()
{
	node* ap = NULL;
	node *bp = NULL;
	int index;
	for (index = 0; index < length; index++)
	{
		if (node_array[index] != NULL)
		{
			if (node_array[index]->data == a)
				ap = node_array[index];
			else if (node_array[index]->data == b)
				bp = node_array[index];
		}
	}//找ap, bp

	while(ap != NULL && findit == 0)//直到双亲为NULL为止，这时候一定是到了根节点
//让ap一直向上走，bp不动
	{
		PreorderTraverse(ap, Visit);

		if (findit)
			printf("%d", ap->data);
		ap = ap->parent;
	}

	

	return;
}

int main()
{
	tree T_for_recur;

	create_array();
	create_tree_array();
	rearrangement();
	create_tree_relationship();
	T_for_recur = root;
	//PreorderTraverse_no_recur(Visit);
	//PreorderTraverse(T_for_recur, Visit);
	//visit_print();

	find_ancestor();

	system("pause");
	return 0;
}