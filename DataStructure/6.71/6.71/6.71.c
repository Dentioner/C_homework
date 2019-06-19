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
	int end;
}node, *tree;

int data_array[MAXLENGTH];
tree node_array[MAXLENGTH];//��������洢ÿ���ڵ��ָ�룬�����ҵ���
int visit_array[MAXLENGTH];
int length = 0;
int length_notNULL = 0;//��¼��0�ڵ����
int visit_counter = 0;
tree root;

void create_array()
{
	int ch;
	int number = -1;
	int index = 0;
	number = getchar();
	ch = getchar();
	while (ch != EOF && ch != '\n' && ch != '\0')
	{

		if (number == 'n')//null
		{
			//ch = getchar();//n
			//ch = getchar();//u
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
		//number = -1;
		number = getchar();
		ch = getchar();
		index++;
	}


	if (number == 'n')//null
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

void create_tree_array()
{
	int index = 0;
	int index2 = 0;
	tree  p;
	root = (tree)malloc(sizeof(node));
	root->lchild = root->rchild = root->parent = NULL;
	root->end = 0;

	node_array[0] = root;
	p = root;

	while (data_array[index] != '\0')
	{
		if (data_array[index] != -1)
		{
			p->data = data_array[index];
			
			index2++;
			if (index2 == length_notNULL)
				p->end = 1;
		}
		else
			p = NULL;
		node_array[index + 1] = p;

		p = (tree)malloc(sizeof(node));
		p->lchild = p->rchild = p->parent = NULL;
		p->data = -1;
		p->end = 0;
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
	visit_array[visit_counter] = e;
	visit_array[visit_counter + 1] = '\0';
	visit_counter++;
	return;
}

void Visit2(node e, int depth)
{
	if (depth == 0)
		printf("%c", e.data);
	else if (depth == 1)
		printf(" %c", e.data);
	else if (depth == 2)
		printf("  %c", e.data);
	else if (depth == 3)
		printf("   %c", e.data);
	else
		printf("    %c", e.data);

	if (!e.end)
		printf("\n");
	return;
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


void PreorderTraverse2(tree T, int depth, void(*Visit2)(node e, int depth))
{
	if (T) 
	{
		Visit2(*T, depth);
		PreorderTraverse2(T->lchild, depth + 1, Visit2);
		PreorderTraverse2(T->rchild, depth, Visit2);
	}
	return;
}


void visit_print()
{
	int index;
	for (index = 0; index < visit_counter - 1; index++)
		printf("%c,", visit_array[index]);
	printf("%c", visit_array[index]);
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

	//PreorderTraverse(T_for_recur, Visit);
	PreorderTraverse2(T_for_recur, 0, Visit2);
	//visit_print();
	system("pause");
	return 0;
}