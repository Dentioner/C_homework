#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


#define BOARD_SIZE 64




typedef struct TNode
{
	bool banned;//��������ڵ��߲�ͨ��
	int row;
	int column;
	struct TNode * children[8];//8������
	struct TNode * parent;
}TreeNode;

void printboard();
void initial_tree();
void init_Node();
void horse();
bool check_coordinate();
void DestroyLeaf(TreeNode*temp_root);
void which_is_the_best_son();


int board[8][8];
TreeNode * Node_now, *Node_before;
int step_counter;
int vector[8][2] = { {-2, 1} ,{-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
TreeNode *root;
int show;//չʾ��
TreeNode *best_son;//̰���㷨ʹ��

int main()
{
	initial_tree();
	printf("Please choose a start TreeNode. Use ',' to distinguish row & column.\n");
	scanf("%d,%d", &(Node_now->row), &(Node_now->column));
	printf("Step by step? Enter 1 means yes and 0 means no.\n");
	scanf("%d", &show);

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

void init_Node()//ÿ�γ�ʼ�������8���ӽڵ�
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
		board[Node_now->row][Node_now->column] = ++step_counter;


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

			Node_now->row = Node_before->row + vector[vector_index][0];
			Node_now->column = Node_before->column + vector[vector_index][1];//������һ��̽��������
			
			if ((!check_coordinate())
				||board[Node_now->row][Node_now->column])//������̵��Ǹ�λ�ò��Ϸ������߲���0
			{
				Node_now->banned = true;
				Node_now = Node_before;//���˵���һ��
				continue;//������һ��vector
			}
			else
			{
				find_it = true;


				board[Node_now->row][Node_now->column] = ++step_counter;
				break;//����ҵ���һ����λ��������ط�д�²���������ѭ�����ڴ˻����Ͻ�����һ��̽��
			}
		}
		//������û���Ż����ı���




		//��������ǳ���ʹ��̰���㷨�Ķ���
		for (vector_index = 0; vector_index < 8; vector_index++)//8���������
		{
			if (Node_now->children[vector_index] != NULL //�����жϸýڵ��ǲ���֮ǰ�������ģ���׼��������û��Ҷ�ڵ� 
				&& Node_now->children[vector_index]->banned)//������ָ÷�λ��֮ǰ������ʱ���Ѿ��߲�ͨ��
				continue;//��������һ����λ

			Node_before = Node_now;//Node_before��ʾ��ǰ�о��Ľڵ�
			init_Node();//��ʼ��ֱ�ӳ�ʼNode_now

			Node_before->children[vector_index] = Node_now;//�ÿսڵ�Ϊ��ǰ�о��ڵ�ĵ�i���ӽڵ�
			Node_now->parent = Node_before;//�������ӹ�ϵ

			Node_now->row = Node_before->row + vector[vector_index][0];
			Node_now->column = Node_before->column + vector[vector_index][1];//������һ��̽��������
			
			if ((!check_coordinate())
				||board[Node_now->row][Node_now->column])//������̵��Ǹ�λ�ò��Ϸ������߲���0
			{
				Node_now->banned = true;
				Node_now = Node_before;//���˵���һ��
				continue;//������һ��vector
			}
			else
			{
				find_it = true;

				//����������Ż�����Ϊ����ط���ʾ���ǣ�����ҵ������ߵĵ����
				//���Ӧ��������ط�дһ���ж������ӽڵ�Ĵ��룬����8��֮���׼��break�����������Ͼ�break
				


				board[Node_now->row][Node_now->column] = ++step_counter;
				break;//����ҵ���һ����λ��������ط�д�²���������ѭ�����ڴ˻����Ͻ�����һ��̽��
			}
		}



		if (!find_it)//���8����������·
		{//������һ�㣬����
			Node_now->banned = true;
			board[Node_now->row][Node_now->column] = 0;//��ԭ
			step_counter--;
			DestroyLeaf(Node_now);

			Node_now = Node_now->parent;//����
		}

		//������չʾ�õģ�����ɾ��
		if (show)
		{
			system("cls");
			printboard();
			system("pause");
		}
	}

	return;
}

bool check_coordinate()//����Ƿ��ǺϷ�����
{
	if (Node_now->row >= 8 || Node_now->row < 0 ||
		Node_now->column >= 8 || Node_now->column < 0)
		return false;
	else
		return true;
}

void DestroyLeaf(TreeNode * temp_root)//�˺���ֻ����Ҷ�ڵ㣬�������ڵ�
{
	TreeNode *temp_leaf;
	for (int i = 0; i < 8; i++)
	{
		temp_leaf = temp_root->children[i];
		if (temp_leaf == NULL)//�����Ҷ�ڵ���NULL��ֱ��������һ��
			continue;
		free(temp_leaf);
	}
	temp_root->banned = true; 
	return;
}

void which_is_the_best_son()
{
	//TreeNode * Node_now, *Node_before;
	TreeNode * Node_grandparents = Node_before;//��Node_beforeΪ�游ĸ�ڵ�
	TreeNode * Node_son;//
	int grandson_number[8];//ͳ��ÿ���ӽڵ�����ӽڵ���
	int tmp_row_of_grandson, tmp_column_of_grandson;//���ӽڵ���ʱ������
	int index_of_bestson = -1;//��ʼ����Ѷ��ӵķ�λ����ʼֵΪ-1

	for (int i = 0; i < 8; i++)//���������ѭ���Ƕ�Node_before��8���������
	{
		if (Node_now->children[i] != NULL //�����жϸýڵ��ǲ���֮ǰ�������ģ���׼��������û��Ҷ�ڵ� 
			&& Node_now->children[i]->banned)//������ָ÷�λ��֮ǰ������ʱ���Ѿ��߲�ͨ��
		{
			grandson_number[i] = 0;//���ӽڵ�û������
			continue;//��������һ����λ
		}
			
		//������ʱ��Node_before�Ѿ�������8���ӽڵ㣬��˲����ٴ������ӽڵ�
		//���Ƕ�ÿ���ӽڵ�������ӽڵ�ͳ��

		Node_son = Node_grandparents->children[i];//Node_sonָ���游ĸ�ڵ�ĵ�i���ӽڵ�
		for (int k = 0; k < 8; k++)//ѭ��8�Σ��о�son�ڵ�����ӽڵ��м���
		{
			//�м��������ߵ����ӽڵ㣬�����м����յķ���
			//�������ֻҪ������ӽڵ������м�����λ����
			//Node_son���ڻ�û�к����ӽڵ㽨����ϵ
			//��ʵҲ���ý�����ϵ��ֱ��ɨ�����̼�������
			tmp_row_of_grandson = Node_son->row + vector[k][0];
			tmp_column_of_grandson = Node_son->column + vector[k][1];
			if (!board[tmp_row_of_grandson][tmp_column_of_grandson])//���������ӽڵ��λ����0����˵����������
			{
				grandson_number[i]++;//������+1, ע��������i����k��k�����ӵ�index��i�Ƕ��ӵ�index
			}
		}
		//ѭ�������󣬵õ��˸ö��ӵ����Ӹ��������ڿ�ʼͳ���ĸ����ӵ���������
		if (index_of_bestson == -1 && grandson_number[i] != 0)
		{
			index_of_bestson = i;//�����Ѷ���index��û�и�ֵ�����ҵ�ǰ�����п����ߵ����ӵĻ����ͳ�ʼ����Ѷ���index
		}
		else
		{
			if (grandson_number[i] < grandson_number[index_of_bestson] && grandson_number[i]!=0)
				index_of_bestson = i;
		}
		
		//������еĶ��ӣ���û�п����ߵ����ӣ���ô��ʱ�����index����-1�����˵������游ĸ�ڵ��߲�ͨ��
		if (index_of_bestson != -1)
			best_son = Node_grandparents->children[index_of_bestson];
		else
			best_son = NULL;//�߲�ͨ��ʱ��bestson = NULL
}