#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
#define MAXLENGTH 300
#define ADDLENGTH 10

typedef int elemtype;

typedef struct TNode
{
	elemtype data;
	struct TNode *lchild, *mchild, *rchild, *parent;
}node, *tree;
//������

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
	scanf("%d", &number);
	ch = getchar();
	while (ch != EOF && ch != '\n' && ch != '\0')
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

void create_tree_array()
{
	int index = 0;
	tree  p;
	root = (tree)malloc(sizeof(node));
	root->lchild = root->rchild = root->parent = root->mchild = NULL;

	node_array[0] = root;
	p = root;

	while (data_array[index] != '\0')
	{
		if (data_array[index] != -1)
			p->data = data_array[index];
		else
			p = NULL;
		node_array[index + 1] = p;

		p = (tree)malloc(sizeof(node));
		p->lchild = p->rchild = p->parent = p->mchild = NULL;
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
		node_array[start + 3] = node_array[start];
		node_array[start] = NULL;
	}
	length += 3;
	return;
}

void rearrangement()
{
//��������ʹ�õ���3����������ע��Ͷ�����������
	tree n, lchild, rchild, mchild;
	int index = 1;
	int counter = 0;
	int lcindex;
	int rcindex;
	int mcindex;
	while (counter < length_notNULL)
	{
		lcindex = 3 * index - 1;
		rcindex = 3 * index + 1;
		mcindex = 3 * index;
		n = node_array[index];
		lchild = node_array[lcindex];
		rchild = node_array[rcindex];
		mchild = node_array[mcindex];
		if (n == NULL)
		{//�Ѻ���Ľڵ�����Ų
			move(index * 3);
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
			n->lchild = node_array[3 * index - 1];
			n->rchild = node_array[3 * index + 1];
			n->mchild = node_array[3 * index];
			if (n->lchild != NULL)
				n->lchild->parent = n;
			if (n->rchild != NULL)
				n->rchild->parent = n;
			if (n->mchild != NULL)
				n->mchild->parent = n;

		}
		index++;



	}

	return;
}

void initial_heap()
{
	int index1;
	tree p = NULL;
	tree lc, mc, rc;
	int tmp = 0;
	lc = mc = rc = NULL;
	index1 = length / 3;
	for (; index1 > 0; index1--)//ע����debug��ʱ��һ������ط�index1��û��ȡ��0������˵����ȡ��0
	{
		p = node_array[index1];
		lc = p->lchild;
		mc = p->mchild;
		rc = p->rchild;
		if (lc != NULL)
		{
			if (lc->data < p->data)
			{
				tmp = p->data;
				p->data = lc->data;
				lc->data = tmp;
			}
		}

		if (mc != NULL)
		{
			if (mc->data < p->data)
			{
				tmp = p->data;
				p->data = mc->data;
				mc->data = tmp;
			}
		}
		if (rc != NULL)
		{
			if (rc->data < p->data)
			{
				tmp = p->data;
				p->data = rc->data;
				rc->data = tmp;
			}
		}


	}
	return;
}

void heap_adjust(int s, int m)
{
	tree rc = node_array[s];
	int j;
	int min_j;
	int tmp1, tmp2;
	for (j = 3 * s - 1; j <= m; j = (j + 1) * 3 - 1)
	{
		//if (j < m && (node_array[j]->data > node_array[j + 1]->data))
			//j++;
		if (j < m)//jҪ������һ���ֵ�����ֵ��С���Ǹ��������
		{
			min_j = j;
			if (node_array[j + 1] != NULL && j + 1 <= m)
			{
				if (node_array[j]->data > node_array[j + 1]->data)
					min_j = j + 1;
			}
			if (node_array[j + 2] != NULL && j + 2 <= m)
			{
				if (node_array[min_j]->data > node_array[j + 2]->data)
					min_j = j + 2;
			}
			j = min_j;
		}

		if (rc->data <= node_array[j]->data)//�������µĴ��뻹�е�����
			break;

		node_array[s] = node_array[j];
		/*
		tmp1 = node_array[s]->data;
		node_array[s]->data = node_array[j]->data;
		node_array[j]->data = tmp1;
		*/
		s = j;
		/*
		tmp2 = s;
		s = j;
		j = tmp2;
		*/
	}
	
	node_array[s] = rc;
	return;
}

void heap_sort()
{
	tree tmp;
	//elemtype tmp;
	//for (int i = length / 3; i > 0; i--)
		//heap_adjust(i, length);
	
	for (int j = length; j > 1; j--)
	{
		tmp = node_array[1];
		//tmp = node_array[1]->data;
		node_array[1] = node_array[j];
		//node_array[1]->data = node_array[j]->data;
		node_array[j] = tmp;
		//node_array[j]->data = tmp;
		visit_array[visit_counter++] = tmp->data;
		//visit_array[visit_counter++] = tmp;

		heap_adjust(1, j - 1);
	}
	visit_array[visit_counter++] = node_array[1]->data;
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

int main()
{
	create_array();
	create_tree_array();
	rearrangement();
	create_tree_relationship();
	initial_heap();
	heap_sort();
	visit_print();
	return 0;
}