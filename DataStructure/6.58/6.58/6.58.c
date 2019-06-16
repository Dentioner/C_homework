#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10
#define link 0
#define thread 1

typedef int elemtype;

typedef struct BiTNode
{
	elemtype data;
	struct BiTNode *lchild, *rchild, *parent;
	int ltag, rtag;
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
	while (ch != EOF && ch != '\n' && ch != '\0' && ch != ';')
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

	



	return;


}

tree root;

void create_tree_array()
{
	int index = 0;
	tree  p;
	root = (tree)malloc(sizeof(node));
	root->lchild = root->rchild = root->parent = NULL;
	root->ltag = root->rtag = 0;
	node_array[0] = root;
	p = root;
	
	//while (data_array[index] != '\0')
	while (index < length)
	{
		if (data_array[index] != -1)
			p->data = data_array[index];
		else
			p = NULL;
		node_array[index + 1] = p;

		p = (tree)malloc(sizeof(node));
		p->lchild = p->rchild = p->parent = NULL;
		p->data = -1;
		p->ltag = p->rtag = 0;
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



void InorderTraverse(tree T, void(*Visit)(elemtype e))
//void InorderTraverse(tree T, void(*Visit)(node * p))
{
	if (T) 
	{
		InorderTraverse(T->lchild, Visit);
		Visit(T->data);
		/*
		Visit(T);
		if (T->rtag == thread && T->rchild == root->lchild)//���һ���ڵ�
		;
		else
		{
			printf(";");
		}
		*/
		InorderTraverse(T->rchild, Visit);
	}
	return;
}

void InorderTraverse_thr(tree T, void(*Visit)(elemtype e))
{
	tree p;
	p = T->lchild;
	while (p != T)
	{
		while (p->ltag == link)
		{
			p = p->lchild;
		}
		Visit(p->data);
		while (p->rtag == thread && p->rchild != T)
		{
			p = p->rchild;
			Visit(p->data);
		}
		p = p->rchild;
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


void add_tree()
{
	int second_tree[MAXLENGTH] = { 0 };
	int length_of_second_tree = 0;
	int insert_point = 0;
	int number, ch;
	scanf("%d", &number);
	ch = getchar();
	while (ch != ';')
	{
		second_tree[length_of_second_tree++] = number;
		scanf("%d", &number);
		ch = getchar();
	}
	second_tree[length_of_second_tree] = number;
	length_of_second_tree++;
	second_tree[length_of_second_tree] = '\0';

	scanf("%d", &insert_point);


	int index = 0;
	tree insert_p, pos;

	while (node_array[index]->data != insert_point)
	{
		index++;
	}
	pos = node_array[index];

	for (int i = 0; i < length_of_second_tree; i++)
	{
		insert_p = (tree)malloc(sizeof(node));
		insert_p->data = second_tree[i];
		insert_p->lchild = insert_p->rchild = insert_p->parent = NULL;
		insert_p->ltag = insert_p->rtag = 0;
		if (pos == node_array[index])
		{
			insert_p->rchild = pos->lchild;
		}	
			insert_p->parent = pos;
			pos->lchild = insert_p;
			pos = insert_p;
		
		

	}


	return;
}
tree pre;

void InThreading(tree p)
{
	
	if (p != NULL)
	{	
		
		InThreading(p->lchild);
		if (p->lchild == NULL)
		{
			p->ltag = thread;
			p->lchild = pre;
		}
		if (pre->rchild == NULL)
		{
			pre->rtag = thread;
			pre->rchild = p;
		}
		pre = p;
		InThreading(p->rchild);
	}
	return;
}

void InorderThreading(tree* head, tree T)
{
	
	(*head) = (tree)malloc(sizeof(node));
	(*head)->parent = NULL;
	(*head)->data = 0;
	(*head)->ltag = link;
	(*head)->rtag = thread;
	(*head)->rchild = (*head);
	if (T == NULL)
		(*head)->lchild = (*head);
	else
	{
		(*head)->lchild = T;
		pre = (*head);
		InThreading(T);
		pre->rchild = (*head);
		pre->rtag = thread;
		(*head)->rchild = pre;
	}
	
	return;
}

void Visit2(node * p)
{
	printf("%d,%d,%d,%d", p->ltag, p->lchild->data, p->rtag, p->rchild->data);
	return;
}

int main()
{
	tree T_for_recur;
	tree head;
	create_array();
	create_tree_array();
	rearrangement();
	create_tree_relationship();
	add_tree();

	T_for_recur = root;
	
	//InorderTraverse(T_for_recur, Visit);
	//visit_print();

	InorderThreading(&head, T_for_recur);
	//InorderTraverse(T_for_recur, Visit2);
	InorderTraverse_thr(T_for_recur, Visit);
	visit_print();

	system("pause");
	return 0;
}