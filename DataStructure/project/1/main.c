#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


#define BOARD_SIZE 20
#define Instruction_ADD 1
#define Instruction_SUB 2



typedef struct TNode
{
	bool banned;//代表这个节点走不通了
	int row;
	int column;
	struct TNode * children[8];//8个子树
	struct TNode * parent;
}TreeNode;

typedef struct Node
{
	int row;
	int column;
}Node;

typedef struct 
{
	Node stack_array[BOARD_SIZE];
	int  top;
} SqStack;

void printboard();

void horse();
bool check_coordinate(Node point);


int board[8][8];
int board_for_next_step[8][8] = {//这个棋盘用来记录某个点下一个可以走的方位有几个
	{2, 3, 4, 4, 4, 4, 3, 2},//每下一个子就得更新一次它
	{3, 4, 6, 6, 6, 6, 4, 3},//这个就是下文提及的孙子棋盘
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{3, 4, 6, 6, 6, 6, 4, 3},
	{2, 3, 4, 4, 4, 4, 3, 2} 
};



int step_counter;
const int vector[8][2] = { {-2, 1} ,{-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };

int show;//展示用

SqStack stack;
Node now, before;


int main()
{
	initial_tree();
	printf("Please choose a start TreeNode. Use ',' to distinguish row & column.\n");
	scanf("%d,%d", &(now.row), &(now.column));
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





void push(Node point)
{
	if (stack.top >= BOARD_SIZE)
	{
		printf("Stack is full, ERROR\n");
		exit(1);
	}
	stack.stack_array[stack.top++] = point;
	return;
}

Node pop()
{
	Node Re;
	if (!stack.top)
	{
		printf("Stack is empty, ERROR\n");
		exit(1);
	}
	Re = stack.stack_array[stack.top--];
	return Re;
}

Node get_top()
{
	Node top;
	if (!stack.top)
	{
		printf("Stack is empty, ERROR\n");
		exit(1);
	}
	top = stack.stack_array[stack.top];
	return top;
}

void Init_Stack()
{
	//stack.stack_array = (Node*)malloc(sizeof(Node)*BOARD_SIZE);
	stack.top = 0;
}

void refresh_next_board(Node point, int instruction)//刷新孙子棋盘
{//instruction 指的是，是要减，还是要加，因为悔棋的时候得恢复孙子个数

	Node next;//这里的next实际上是以now为下一步的那些点
	for (int vector_index = 0; vector_index < 8; vector_index++)//8个方向遍历
	{
		next.row = point.row + vector[vector_index][0];
		next.column = point.column + vector[vector_index][1];//生成坐标
		if (!check_coordinate(next))
		{
			continue;
		}
		if (instruction == Instruction_SUB)
		{
			board_for_next_step[next.row][next.column] --;
		}
		else
		{
			board_for_next_step[next.row][next.column] ++;
		}
	}

	return;
}

bool check_coordinate(Node point)//检查是否是合法坐标
{
	if (point.row >= 8 || point.row < 0 || point.column >= 8 || point.column < 0 
		|| board[point.row][point.column])//如果棋盘的那个位置不合法，或者不是0
		return false;
	else
		return true;
}

Node next_step(Node point)//找下一步应该去哪
{//使用贪心算法，检索孙子棋盘
	Node next;
	Node best_son;
	bool has_been_initialized = false;//最佳儿子是否被初始化了
	for (int vector_index = 0; vector_index < 8; vector_index++)//8个方向遍历
	{
		next.row = point.row + vector[vector_index][0];
		next.column = point.column + vector[vector_index][1];//生成坐标
		if (!check_coordinate(next))
		{
			continue;
		}
		if (!has_been_initialized)//如果之前还未初始化最佳儿子，这里就初始化
		{
			best_son = next;
			has_been_initialized = true;
		}
		else
		{
			if (board_for_next_step[next.row][next.column]
				< board_for_next_step[best_son.row][best_son.column])
			{
				best_son = next;
			}
		}
	}

	return best_son;
}

void horse()
{
	int vector_index;
	bool find_it;
	if (!step_counter)//第一步，把选定的地方写上1
	{
		board[now.row][now.column] = ++step_counter;
		push(now);
		refresh_next_board(now, Instruction_SUB);
	}
	
	while (stack.top)//只要栈不为空，就一直可以跑下去
	{
		if (!board_for_next_step[now.row][now.column])//如果现在没有子节点可以走了
		{
			board[now.row][now.column] = 0;
			step_counter--;

		}

		
		
		if (check_coordinate(now))//如果当前位置合法
		{
			push(now);
			board[now.row][now.column] = step_counter++;
			before = now;
			refresh_next_board(before, Instruction_SUB);
			now = next_step(before);
		}


	}




		//下面是展示用的，可以删掉
		if (show)
		{
			system("cls");
			printboard();
			system("pause");
		}
	

	return;
}





