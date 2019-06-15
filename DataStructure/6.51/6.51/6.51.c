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
	int tag;//新增标志域，区别这个节点是操作数还是操作符
	struct BiTNode *lchild, *rchild, *parent;
}node, *tree;


int data_array[MAXLENGTH];
tree node_array[MAXLENGTH];//这个用来存储每个节点的指针，方便找到它
int visit_array[MAXLENGTH];
int length = 0;
int length_notNULL = 0;//记录非0节点个数
int visit_counter = 0;
int this_position_is_not_a_number[MAXLENGTH] = { 0 };//记录data_array里面哪些位置是符号
//如果某个位置不是数字，就记为1
int this_position_is_not_a_number_ver2[MAXLENGTH] = { 0 };//after rearrangement()


//比如说我想找编号为12的节点，直接在这个地方找nodearray[12]就行了
//nodearray[0]存放长度

void create_array()
{
	int ch, ch2;
	int number = -1;
	char numberc;
	int index = 0;
	int scanf_detector, scanf_detector2;//判断这一波是符号还是数字
	scanf_detector2 = scanf("%c", &numberc);//很奇怪，读到+-号会跳到后一个去，但是*、/不会
	
	if (numberc == '+' || numberc == '-')
	{
		scanf_detector = 0;
		ch = getchar();//读'，'这个符号，将这个符号从缓冲区拿掉
		ch = (int)numberc;
	}
	else if (numberc == '*' || numberc == '/')
	{
		scanf_detector = 0;
		ch = getchar();//读'，'这个符号，将这个符号从缓冲区拿掉
		ch = (int)numberc;
	}
	else
	{
		scanf_detector =  scanf("%d", &number);
		ch = getchar();//ch一定能读到','
		if (number == -1)//只有一位数
		{
			number = (int)(numberc - '0');
		}
		else//两位数
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
		if (scanf_detector || ch == 'n')//读到的是数字而不是符号，或者读到的是null的首字母n
		{
			if (number == -1)//null
			{
				//ch = getchar();//n
				//ch = getchar();//u
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

		}
		else//读到符号了
		{//现在ch就是那个符号
			data_array[index] = ch;
			length_notNULL++;
			this_position_is_not_a_number[index] = 1;
			if (ch != '+' && ch != '-' && ch != '*' && ch != '/')//如果ch之前是+-号，那么后面的那个逗号就读过了，不用再刷新一次了
				ch = getchar();//读'，'这个符号，将这个符号从缓冲区拿掉
		}
		//下一个
		number = -1;
		scanf_detector2 = scanf("%c", &numberc);//很奇怪，读到+-号会跳到后一个去，但是*、/不会
		ch2 = getchar();//读取可能存在的\n符号

		if (numberc == '+' || numberc == '-')
		{
			scanf_detector = 0;
			ch = ch2;//读'，'这个符号，将这个符号从缓冲区拿掉
			ch = (int)numberc;
		}
		else if (numberc == 'n')//null
		{
			ch = (int)numberc;//注意，ch2已经将null里面的u读了
			scanf_detector = 0;
		}
		else if (numberc == '*' || numberc == '/')
		{
			scanf_detector = 0;
			ch = ch2;//读'，'这个符号，将这个符号从缓冲区拿掉
			ch = (int)numberc;
		}
		else
		{
			
			//scanf_detector = scanf("%d", &number);
			//ch = getchar();//ch一定能读到','
			if ('0' > ch2 || ch2 > '9')//只有一位数
			{
				number = (int)(numberc - '0');
				scanf_detector = 1;
				ch = ch2;
			}
			else//两位数
			{
				number = (int)(numberc - '0') * 10 + (int)(ch2 - '0');
				scanf_detector = 1;
				ch = getchar();//ch把那个剩下来的符号给读了
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

		data_array[index++] = -1;//用-1表示null        
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
		//这里试着加上一个打印括号的内容
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