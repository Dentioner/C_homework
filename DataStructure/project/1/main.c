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
	bool banned;//代表这个节点走不通了
	int raw;
	int column;
	struct TNode * children[8];//8个子树
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
	if (!step_counter)//第一步，把选定的地方写上1
	{
		board[Node_now->raw][Node_now->column] = ++step_counter;


	}
	while (step_counter < BOARD_SIZE)//当步数超过64步的时候停下来
	{
		find_it = false;
		for (vector_index = 0; vector_index < 8; vector_index++)//8个方向遍历
		{
			if (Node_now->children[vector_index] != NULL //首先判断该节点是不是之前遍历过的，标准就是它有没有叶节点 
				&& Node_now->children[vector_index]->banned)//如果发现该方位在之前遍历的时候已经走不通了
				continue;//就跳到下一个方位

			Node_before = Node_now;
			init_Node();

			Node_before->children[vector_index] = Node_now;
			Node_now->parent = Node_before;//生成亲子关系

			Node_now->raw = Node_before->raw + vector[vector_index][0];
			Node_now->column = Node_before->column + vector[vector_index][1];//生成下一步探索的坐标
			
			if ((!check_coordinate())
				||board[Node_now->raw][Node_now->column])//如果棋盘的那个位置不合法，或者不是0
			{
				Node_now->banned = true;
				Node_now = Node_before;//撤退到上一层
				continue;//跳到下一个vector
			}
			else
			{
				find_it = true;


				board[Node_now->raw][Node_now->column] = ++step_counter;
				break;//如果找到了一个空位，则将这个地方写下步数，跳出循环，在此基础上进行下一步探索
			}
		}

		if (!find_it)//如果8个方向都是死路
		{//返回上一层，回溯
			Node_now->banned = true;
			board[Node_now->raw][Node_now->column] = 0;//还原
			step_counter--;
			Node_now = Node_now->parent;//回溯
		}

	}

	return;
}

bool check_coordinate()//检查是否是合法坐标
{
	if (Node_now->raw >= 8 || Node_now->raw < 0 ||
		Node_now->column >= 8 || Node_now->column < 0)
		return false;
	else
		return true;
}