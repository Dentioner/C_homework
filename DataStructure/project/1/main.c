#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


#define BOARD_SIZE 64




typedef struct TNode
{
	bool banned;//代表这个节点走不通了
	int row;
	int column;
	struct TNode * children[8];//8个子树
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
int show;//展示用
TreeNode *best_son;//贪心算法使用

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

void init_Node()//每次初始化都搞出8个子节点
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
		board[Node_now->row][Node_now->column] = ++step_counter;


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

			Node_now->row = Node_before->row + vector[vector_index][0];
			Node_now->column = Node_before->column + vector[vector_index][1];//生成下一步探索的坐标
			
			if ((!check_coordinate())
				||board[Node_now->row][Node_now->column])//如果棋盘的那个位置不合法，或者不是0
			{
				Node_now->banned = true;
				Node_now = Node_before;//撤退到上一层
				continue;//跳到下一个vector
			}
			else
			{
				find_it = true;


				board[Node_now->row][Node_now->column] = ++step_counter;
				break;//如果找到了一个空位，则将这个地方写下步数，跳出循环，在此基础上进行下一步探索
			}
		}
		//上面是没有优化过的遍历




		//下面这个是尝试使用贪心算法的东西
		for (vector_index = 0; vector_index < 8; vector_index++)//8个方向遍历
		{
			if (Node_now->children[vector_index] != NULL //首先判断该节点是不是之前遍历过的，标准就是它有没有叶节点 
				&& Node_now->children[vector_index]->banned)//如果发现该方位在之前遍历的时候已经走不通了
				continue;//就跳到下一个方位

			Node_before = Node_now;//Node_before表示当前研究的节点
			init_Node();//初始化直接初始Node_now

			Node_before->children[vector_index] = Node_now;//该空节点为当前研究节点的第i个子节点
			Node_now->parent = Node_before;//生成亲子关系

			Node_now->row = Node_before->row + vector[vector_index][0];
			Node_now->column = Node_before->column + vector[vector_index][1];//生成下一步探索的坐标
			
			if ((!check_coordinate())
				||board[Node_now->row][Node_now->column])//如果棋盘的那个位置不合法，或者不是0
			{
				Node_now->banned = true;
				Node_now = Node_before;//撤退到上一层
				continue;//跳到下一个vector
			}
			else
			{
				find_it = true;

				//在这里进行优化，因为这个地方表示的是，如果找到可以走的点就走
				//因此应该在这个地方写一个判断最优子节点的代码，遍历8次之后才准备break，而不是马上就break
				


				board[Node_now->row][Node_now->column] = ++step_counter;
				break;//如果找到了一个空位，则将这个地方写下步数，跳出循环，在此基础上进行下一步探索
			}
		}



		if (!find_it)//如果8个方向都是死路
		{//返回上一层，回溯
			Node_now->banned = true;
			board[Node_now->row][Node_now->column] = 0;//还原
			step_counter--;
			DestroyLeaf(Node_now);

			Node_now = Node_now->parent;//回溯
		}

		//下面是展示用的，可以删掉
		if (show)
		{
			system("cls");
			printboard();
			system("pause");
		}
	}

	return;
}

bool check_coordinate()//检查是否是合法坐标
{
	if (Node_now->row >= 8 || Node_now->row < 0 ||
		Node_now->column >= 8 || Node_now->column < 0)
		return false;
	else
		return true;
}

void DestroyLeaf(TreeNode * temp_root)//此函数只砍掉叶节点，保留根节点
{
	TreeNode *temp_leaf;
	for (int i = 0; i < 8; i++)
	{
		temp_leaf = temp_root->children[i];
		if (temp_leaf == NULL)//如果该叶节点是NULL，直接跳到下一个
			continue;
		free(temp_leaf);
	}
	temp_root->banned = true; 
	return;
}

void which_is_the_best_son()
{
	//TreeNode * Node_now, *Node_before;
	TreeNode * Node_grandparents = Node_before;//令Node_before为祖父母节点
	TreeNode * Node_son;//
	int grandson_number[8];//统计每个子节点的孙子节点数
	int tmp_row_of_grandson, tmp_column_of_grandson;//孙子节点临时的坐标
	int index_of_bestson = -1;//初始化最佳儿子的方位，初始值为-1

	for (int i = 0; i < 8; i++)//首先最外层循环是对Node_before的8个方向遍历
	{
		if (Node_now->children[i] != NULL //首先判断该节点是不是之前遍历过的，标准就是它有没有叶节点 
			&& Node_now->children[i]->banned)//如果发现该方位在之前遍历的时候已经走不通了
		{
			grandson_number[i] = 0;//该子节点没有孙子
			continue;//就跳到下一个方位
		}
			
		//由于这时候Node_before已经产生了8个子节点，因此不用再次生成子节点
		//而是对每个子节点进行孙子节点统计

		Node_son = Node_grandparents->children[i];//Node_son指向祖父母节点的第i个子节点
		for (int k = 0; k < 8; k++)//循环8次，研究son节点的孙子节点有几个
		{
			//有几个可以走的孙子节点，就是有几个空的方向
			//因此现在只要看这个子节点四周有几个空位就行
			//Node_son现在还没有和孙子节点建立关系
			//其实也不用建立关系，直接扫描棋盘计数就行
			tmp_row_of_grandson = Node_son->row + vector[k][0];
			tmp_column_of_grandson = Node_son->column + vector[k][1];
			if (!board[tmp_row_of_grandson][tmp_column_of_grandson])//如果这个孙子节点的位置是0，则说明可以走这
			{
				grandson_number[i]++;//孙子数+1, 注意这里是i不是k，k是孙子的index，i是儿子的index
			}
		}
		//循环结束后，得到了该儿子的孙子个数，现在开始统计哪个儿子的孙子最少
		if (index_of_bestson == -1 && grandson_number[i] != 0)
		{
			index_of_bestson = i;//如果最佳儿子index还没有赋值，而且当前儿子有可以走的孙子的话，就初始化最佳儿子index
		}
		else
		{
			if (grandson_number[i] < grandson_number[index_of_bestson] && grandson_number[i]!=0)
				index_of_bestson = i;
		}
		
		//如果所有的儿子，都没有可以走的孙子，那么这时候儿子index还是-1，这就说明这个祖父母节点走不通了
		if (index_of_bestson != -1)
			best_son = Node_grandparents->children[index_of_bestson];
		else
			best_son = NULL;//走不通的时候bestson = NULL
}