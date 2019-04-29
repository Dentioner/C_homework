#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


#define BOARD_SIZE 63

void printboard();
void initial_tree();
void init_Node();
void horse();
bool check_coordinate();

typedef struct TNode
{
	bool banned;//��������ڵ��߲�ͨ��
	int raw;
	int column;
	struct TNode * children[8];//8������
	struct TNode * parent;
}TreeNode;

int board[8][8];
TreeNode * Node_now, *Node_before;
int step_counter;
int vector[8][2] = { {-2, 1} ,{-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
TreeNode *root;



int main()
{
	initial_tree();
	printf("Please choose a start TreeNode. Use ',' to distinguish raw & column.\n");
	scanf("%d,%d", &(Node_now->raw), &(Node_now->column));
	horse();

    printboard();
    system("pause");
    return 0;
}


void printboard()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%-4d", board[i][j]);
		}
		printf("\n");
	}
	return;
}

void initial_tree()
{
	init_Node();
	root = Node_now;
	root->parent = NULL;
	return;
}

void init_Node()
{
	Node_now = (TreeNode*)malloc(sizeof(TreeNode));
	if (Node_now == NULL)
	{
		printf("malloc failed.\n");
		exit(1);
	}
	for (int i = 0; i < 8; i++)
		Node_now->children[i] = NULL;
	return;
}

/*
void push(TreeNode local)
{
	if ((leaf - root) > (BOARD_SIZE * sizeof(TreeNode)))
	{
		printf("Error1\n");
		exit(1);
	}
	*leaf = local;
	leaf++;
	return;
}

TreeNode pop()
{
	if (leaf == root)
	{
		printf("Error2\n");
		exit(1);
	}
	leaf--;
	return *leaf;
}
*/
void horse()
{
	int vector_index;
	bool find_it;
	if (!step_counter)//��һ������ѡ���ĵط�д��1
	{
		board[Node_now->raw][Node_now->column] = ++step_counter;


	}
	while (step_counter < BOARD_SIZE)//����������64����ʱ��ͣ����
	{
		find_it = false;
		for (vector_index = 0; vector_index < 8; vector_index++)//8���������
		{
			if (Node_now->children[vector_index] != NULL //�����жϸýڵ��ǲ���֮ǰ�������ģ���׼��������û��Ҷ�ڵ� 
				&& Node_now->children[vector_index]->banned)//������ָ÷�λ��֮ǰ������ʱ���Ѿ��߲�ͨ��
				continue;//��������һ����λ

			Node_before = Node_now;
			init_Node();

			Node_before->children[vector_index] = Node_now;
			Node_now->parent = Node_before;//�������ӹ�ϵ

			Node_now->raw = Node_before->raw + vector[vector_index][0];
			Node_now->column = Node_before->column + vector[vector_index][1];//������һ��̽��������
			
			if ((!check_coordinate())
				||board[Node_now->raw][Node_now->column])//������̵��Ǹ�λ�ò��Ϸ������߲���0
			{
				Node_now->banned = true;
				Node_now = Node_before;//���˵���һ��
				continue;//������һ��vector
			}
			else
			{
				find_it = true;


				board[Node_now->raw][Node_now->column] = ++step_counter;
				break;//����ҵ���һ����λ��������ط�д�²���������ѭ�����ڴ˻����Ͻ�����һ��̽��
			}
		}

		if (!find_it)//���8����������·
		{//������һ�㣬����
			Node_now->banned = true;
			board[Node_now->raw][Node_now->column] = 0;//��ԭ
			step_counter--;
			Node_now = Node_now->parent;//����
		}

	}

	return;
}

bool check_coordinate()//����Ƿ��ǺϷ�����
{
	if (Node_now->raw >= 8 || Node_now->raw < 0 ||
		Node_now->column >= 8 || Node_now->column < 0)
		return false;
	else
		return true;
}