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
	int top; //ջ��
	tree *base;
	int stacksize;
}SqStack; //��̬˳��ջ

int data_array[MAXLENGTH];
tree node_array[MAXLENGTH];//��������洢ÿ���ڵ��ָ�룬�����ҵ���
int visit_array[MAXLENGTH];
int length = 0;
int length_notNULL = 0;//��¼��0�ڵ����
int visit_counter = 0;
int a, b;
int findit = 0;
//����˵�����ұ��Ϊ12�Ľڵ㣬ֱ��������ط���nodearray[12]������
//nodearray[0]��ų���

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

			//ע������ط�û�п������һ������Ϊnull�����
			//������һ������Ϊnull����ô��һ���ַ��Ͳ��Ƕ��Ŷ��ǻس�
			ch = getchar();//,
			if (ch == '\n')
				break;


			data_array[index] = -1;//��-1��ʾnull        
		}
		else
		{
			data_array[index] = number;
			length_notNULL++;
		}


		//��һ��
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

		data_array[index++] = -1;//��-1��ʾnull        
	}
	else
	{
		data_array[index++] = number;
		length_notNULL++;
	}

	data_array[index] = '\0';

	length = index;

	scanf("%d;%d", &a, &b);//�����ȵ�������
	


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

void rearrangement()//������������ݲ���ȫ�ǰ��ղ�α�����ȫ�����������ֱ������
{//�����Ҫ����������Ϊ��ȫ����������������
//����˵���������1,2,3,null,null,null,4,null,5
//����2���¼�ȫ��null��ʵ��������ʡȥ�˺ö��null
//5ʵ����Ӧ�ó����ڵ�15��λ�òŶ�
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
		{//�Ѻ���Ľڵ�����Ų
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
	{ //ջ��
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
	while (p != NULL || !IsStackEmpty(&s))//ͬʱ�����˵��������
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
	}//��ap, bp

	while(ap != NULL && findit == 0)//ֱ��˫��ΪNULLΪֹ����ʱ��һ���ǵ��˸��ڵ�
//��apһֱ�����ߣ�bp����
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