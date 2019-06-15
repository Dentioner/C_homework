#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10
#define num 0
#define symbol 1

typedef int elemtype;

typedef struct BiTNode
{
	elemtype data;
	int tag;//������־����������ڵ��ǲ��������ǲ�����
	struct BiTNode *lchild, *rchild, *parent;
}node, *tree;


int data_array[MAXLENGTH];
tree node_array[MAXLENGTH];//��������洢ÿ���ڵ��ָ�룬�����ҵ���
int visit_array[MAXLENGTH];
int length = 0;
int length_notNULL = 0;//��¼��0�ڵ����
int visit_counter = 0;
int this_position_is_not_a_number[MAXLENGTH] = { 0 };//��¼data_array������Щλ���Ƿ���
//���ĳ��λ�ò������֣��ͼ�Ϊ1
int this_position_is_not_a_number_ver2[MAXLENGTH] = { 0 };//after rearrangement()


//����˵�����ұ��Ϊ12�Ľڵ㣬ֱ��������ط���nodearray[12]������
//nodearray[0]��ų���

void create_array()
{
	int ch, ch2;
	int number = -1;
	char numberc;
	int index = 0;
	int scanf_detector, scanf_detector2;//�ж���һ���Ƿ��Ż�������
	scanf_detector2 = scanf("%c", &numberc);//����֣�����+-�Ż�������һ��ȥ������*��/����
	
	if (numberc == '+' || numberc == '-')
	{
		scanf_detector = 0;
		ch = getchar();//��'��'������ţ���������Ŵӻ������õ�
		ch = (int)numberc;
	}
	else if (numberc == '*' || numberc == '/')
	{
		scanf_detector = 0;
		ch = getchar();//��'��'������ţ���������Ŵӻ������õ�
		ch = (int)numberc;
	}
	else
	{
		scanf_detector =  scanf("%d", &number);
		ch = getchar();//chһ���ܶ���','
		if (number == -1)//ֻ��һλ��
		{
			number = (int)(numberc - '0');
		}
		else//��λ��
		{
			number += (int)(numberc - '0')*10;
		}
	}
	
	/*
	if (scanf_detector == 0 && ch == ',')
	{
		ch = (int)'+';
	}
	*/
	while (ch != EOF && ch != '\n' && ch != '\0' && ch != ';')
	{
		if (scanf_detector || ch == 'n')//�����������ֶ����Ƿ��ţ����߶�������null������ĸn
		{
			if (number == -1)//null
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

		}
		else//����������
		{//����ch�����Ǹ�����
			data_array[index] = ch;
			length_notNULL++;
			this_position_is_not_a_number[index] = 1;
			if (ch != '+' && ch != '-' && ch != '*' && ch != '/')//���ch֮ǰ��+-�ţ���ô������Ǹ����žͶ����ˣ�������ˢ��һ����
				ch = getchar();//��'��'������ţ���������Ŵӻ������õ�
		}
		//��һ��
		number = -1;
		scanf_detector2 = scanf("%c", &numberc);//����֣�����+-�Ż�������һ��ȥ������*��/����
		ch2 = getchar();//��ȡ���ܴ��ڵ�\n����

		if (numberc == '+' || numberc == '-')
		{
			scanf_detector = 0;
			ch = ch2;//��'��'������ţ���������Ŵӻ������õ�
			ch = (int)numberc;
		}
		else if (numberc == 'n')//null
		{
			ch = (int)numberc;//ע�⣬ch2�Ѿ���null�����u����
			scanf_detector = 0;
		}
		else if (numberc == '*' || numberc == '/')
		{
			scanf_detector = 0;
			ch = ch2;//��'��'������ţ���������Ŵӻ������õ�
			ch = (int)numberc;
		}
		else
		{
			
			//scanf_detector = scanf("%d", &number);
			//ch = getchar();//chһ���ܶ���','
			if ('0' > ch2 || ch2 > '9')//ֻ��һλ��
			{
				number = (int)(numberc - '0');
				scanf_detector = 1;
				ch = ch2;
			}
			else//��λ��
			{
				number = (int)(numberc - '0') * 10 + (int)(ch2 - '0');
				scanf_detector = 1;
				ch = getchar();//ch���Ǹ�ʣ�����ķ��Ÿ�����
			}
			
			
		}
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

	node_array[0] = root;
	p = root;

	//while (data_array[index] != '\0')
	while (index < length)
	{
		if (data_array[index] != -1)
		{
			p->data = data_array[index];
			p->tag = this_position_is_not_a_number[index];
		}
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

	visit_array[visit_counter] = e;
	visit_array[visit_counter + 1] = '\0';
	visit_counter++;
	return;
}

int compare_symbol(int a, int b)
{
	char cha = (char)a;
	char chb = (char)b;
	//if ((cha == '*' || cha == '/') && (chb == '+' || chb == '-'))
	if ((cha == '*' || cha == '/' || cha == '-') && (chb == '+' || chb == '-'))
		return 1;
	
	else
		return 0;
}

void InorderTraverse(tree T, void(*Visit)(elemtype e))
{
	if (T) 
	{
	
		InorderTraverse(T->lchild, Visit);
		this_position_is_not_a_number_ver2[visit_counter] = T->tag;
		
		Visit(T->data);

		if (T->tag == symbol && T->rchild->tag == symbol)
			if (compare_symbol(T->data, T->rchild->data))
			{
				Visit((int)'(');
				this_position_is_not_a_number_ver2[visit_counter- 1] = 1;
			}

		InorderTraverse(T->rchild, Visit);
		//�������ż���һ����ӡ���ŵ�����
		if (T->tag == symbol && T->rchild->tag == symbol)
			if (compare_symbol(T->data, T->rchild->data))
			{
				Visit((int)')');
				this_position_is_not_a_number_ver2[visit_counter - 1] = 1;
			} 
	}
	return;
}


void visit_print()
{
	int index;
	//for (index = 0; index < visit_counter - 1; index++)
		//printf("%d,", visit_array[index]);
	//printf("%d", visit_array[index]);
	for (index = 0; index < visit_counter; index++)
	{
		if (this_position_is_not_a_number_ver2[index] == symbol)
			printf("%c", visit_array[index]);
		else
			printf("%d", visit_array[index]);
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
	InorderTraverse(T_for_recur, Visit);
	visit_print();

	

	system("pause");
	return 0;
}