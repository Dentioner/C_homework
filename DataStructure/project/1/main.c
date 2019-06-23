#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>


#define BOARD_SIZE 64
#define Instruction_ADD 1
#define Instruction_SUB 2
#define MANUAL 2
#define AUTO 1
#define TIME_LIMIT 1000 //单位：ms


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
	int son[8][2];//记录儿子数组,每个son[i][0]记录儿子相对于此节点的方位，即vector数组的索引值
	//每个son[i][1]记录儿子含有的孙子个数
	//例如，如果最佳儿子是位于本节点的右边的最下方那个4号节点，该节点就2个孙子，那么s[0][1]=4，s[0][2] = 2;
	//如果某个儿子走不通，那么它的孙子个数为9以上，以便排序的时候能够放在最后考虑
	int back_track_counter;//记录回溯次数，同时此变量也可作为下一步访问的儿子数组的元素索引
}Node;

typedef struct 
{
	Node* stack_array[BOARD_SIZE];
	int  top;
} SqStack;

void printboard();
void push(Node* point_pointer);
Node* pop();
Node* get_top();
void Init_Stack();
void refresh_next_board(Node* point, int instruction);
void horse();
bool check_coordinate(Node testpoint);
Node* next_step(Node* point);
void Init_nodeboard();
void wait();
void show_printer();
void get_input();

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


int print_counter;
int step_counter;
const int vector[8][2] = { {-2, 1} ,{-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };

int show;//展示用
int mode_number;
int solution_upper_bound;

bool no_more_grandson = false;//next_step用

SqStack stack;
Node *now, *before;

Node nodeboard[8][8];

int main()
{
	Init_nodeboard();
	Init_Stack();
	
	get_input();

	horse();

    //printboard();
    system("pause");
    return 0;
}


void get_input()
{
	int first_row, first_column;
	int input_index;
	int invalid_input = 1;
	while (invalid_input)
	{
		printf("Please choose a start TreeNode. Use ',' to distinguish row & column.\n");
		input_index = scanf("%d,%d", &(first_row), &(first_column));
		while (getchar() != '\n')
			continue;
		if (input_index != 2)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}
		
		if (first_row < 0 || first_row >= 8)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}
		if (first_column < 0 || first_column >= 8)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}
		
		invalid_input = 0;
	}

	invalid_input = 1;
	while (invalid_input)
	{
		printf("Step by step? Enter 1 means yes and 0 means no.\n");
		input_index = scanf("%d", &show);
		while (getchar() != '\n')
			continue;
		if (input_index != 1)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}

		if (show != 0 && show != 1)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}

		invalid_input = 0;
	}


	if (show)
	{
		invalid_input = 1;
		while (invalid_input)
		{
			printf("Auto mode(1) or Manual mode(2)?\n");
			input_index = scanf("%d", &mode_number);
			while (getchar() != '\n')
				continue;
			if (input_index != 1)
			{
				printf("Invalid input. Try again.\n");
				continue;
			}
			if (mode_number != 1 && mode_number != 2)
			{
				printf("Invalid input. Try again.\n");
				continue;
			}
			invalid_input = 0;
		}
	}
	else
		mode_number = 0;
	invalid_input = 1;
	while (invalid_input)
	{
		printf("How many solutions?\n");
		input_index = scanf("%d", &solution_upper_bound);
		while (getchar() != '\n')
			continue;
		if (input_index != 1)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}
		
		if (solution_upper_bound < 0)
		{
			printf("Invalid input. Try again.\n");
			continue;
		}

		invalid_input = 0;
	}

	now = &nodeboard[first_row][first_column];

	return;
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





void push(Node *point_pointer)
{
	if (stack.top >= BOARD_SIZE)
	{
		printf("Stack is full, ERROR\n");
		exit(1);
	}
	stack.stack_array[++stack.top] = point_pointer;
	return;
}

Node* pop()
{
	Node* Re;
	if (stack.top == -1)
	{
		printf("Stack is empty, ERROR\n");
		exit(1);
	}
	Re = stack.stack_array[stack.top--];
	return Re;
}

Node* get_top()
{
	Node* top;
	if (stack.top == -1)
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
	stack.top = -1;
}

void refresh_next_board(Node* point, int instruction)//刷新孙子棋盘
{//instruction 指的是，是要减，还是要加，因为悔棋的时候得恢复孙子个数
	Node testnode;
	Node* next;//这里的next实际上是以now为下一步的那些点
	for (int vector_index = 0; vector_index < 8; vector_index++)//8个方向遍历
	{
		testnode.row = point->row + vector[vector_index][0];
		testnode.column = point->column + vector[vector_index][1];
		next = &nodeboard[point->row + vector[vector_index][0]][point->column + vector[vector_index][1]];
		if (!check_coordinate(testnode))
		{
			continue;
		}
		if (instruction == Instruction_SUB)
		{
			board_for_next_step[next->row][next->column] --;
		}
		else
		{
			board_for_next_step[next->row][next->column] ++;
		}
	}

	return;
}

bool check_coordinate(Node testpoint)//检查是否是合法坐标
{
	if (testpoint.row >= 8 || testpoint.row < 0 || testpoint.column >= 8 || testpoint.column < 0
		|| board[testpoint.row][testpoint.column])//如果棋盘的那个位置不合法，或者不是0
		return false;
	else
		return true;
}

Node* next_step(Node* point)//找下一步应该去哪
{//使用贪心算法，检索孙子棋盘
	//在此步内生成每个Node的儿子数组
	Node testnode;
	Node* next;
	Node* best_son;
	bool has_been_initialized = false;//最佳儿子是否被初始化了
	int i = 0;
	int vector_index = 0;
	//首先在生成儿子之前先看看有没有之前生成过儿子了，否则就不用再跑一次循环
	//判断之前有没有生成儿子的方式，我觉得可以根据Node结构里面的那个回溯计数器来搞

	if (point->row == 2 && point->column == 1 && step_counter == 2)
		printf("\n");


	if (!point->back_track_counter)//计数器为0说明没有回溯过
	{
		for (; vector_index < 8; vector_index++)//8个方向遍历
		{
			testnode.row = point->row + vector[vector_index][0];
			testnode.column = point->column + vector[vector_index][1];
			next = &nodeboard[point->row + vector[vector_index][0]][point->column + vector[vector_index][1]];
			point->son[i][0] = vector_index;


			if (!check_coordinate(testnode))
			{
				point->son[i++][1] = 9 + vector_index;//瞎写的一个大于9的数值			
				continue;
			}
			else
			{
				point->son[i++][1] = board_for_next_step[next->row][next->column];
			}
		}
		//跑完8次之后执行排序

		int ii, jj, key, key_vec;
		for (ii = 1; ii < 8; ii++)
		{
			key_vec = point->son[ii][0];
			key = point->son[ii][1];
			jj = ii - 1;
			while ((jj >= 0) && (point->son[jj][1] > key))
			{
				point->son[jj + 1][0] = point->son[jj][0];
				point->son[jj + 1][1] = point->son[jj][1];

				jj--;
			}
			point->son[jj + 1][0] = key_vec;
			point->son[jj + 1][1] = key;

		}//排序
		
		//******这个地方似乎没有检查生成的那一长串坐标值，是不是正常的
		best_son = 
			&nodeboard
			[point->row + vector[point->son[point->back_track_counter][0]][0]]
			[point->column + vector[point->son[point->back_track_counter][0]][1]];
		
		if (point->son[point->back_track_counter][1] > 8)//说明没有更多的孙子了，大于8的都是之前预设的垃圾值
		{
			no_more_grandson = true;

		}
		else
		{
			no_more_grandson = false;
		}
			
		point->back_track_counter++;
	}

	else//back_track_counter != 0
	{
		//******这个地方似乎没有检查生成的那一长串坐标值，是不是正常的
		best_son =
			&nodeboard
			[point->row + vector[point->son[point->back_track_counter][0]][0]]
			[point->column + vector[point->son[point->back_track_counter][0]][1]];

		if (point->son[point->back_track_counter][1] > 8)//说明没有更多的孙子了，大于8的都是之前预设的垃圾值
		{
			no_more_grandson = true;

		}
		else
		{
			no_more_grandson = false;
		}

		point->back_track_counter++;
	}


	return best_son;
}

void Init_nodeboard()
{
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			nodeboard[i][j].row = i;
			nodeboard[i][j].column = j;
			nodeboard[i][j].back_track_counter = 0;
		}
	}
	return;
}

void wait()
{
	double start_time;
	double current_time;
	start_time = clock();
	current_time = clock();
	while (current_time - start_time < TIME_LIMIT)
	{
		current_time = clock();
		continue;
	}

	return;
}

void show_printer()
{
	
	system("cls");
	printf("=============================backtracking mode=============================\n");
	printf("automode:");
	if (mode_number == AUTO)
		printf("on\n");
	else
		printf("off\n");
	printf("第%d次\n", print_counter + 1);
	printboard();
	if (mode_number == MANUAL)
		system("pause");
	else
		wait();
	
}

void horse()
{
	//int vector_index;
	//bool find_it;
	Node testnode;
	Node* Re;//用于存储pop的返回值
	if (!step_counter)//第一步，把选定的地方写上1
	{
		board[now->row][now->column] = ++step_counter;
		push(now);
		refresh_next_board(now, Instruction_SUB);
		before = now;
		now = next_step(before);
	}
	if (!show)
	{
		system("cls");
		printf("=============================illustration mode=============================\n");
	}

	while (stack.top!=-1 && print_counter < solution_upper_bound)//只要栈不为空，就一直可以跑下去
	{

		testnode.row = now->row;
		testnode.column = now->column;

		if (step_counter == BOARD_SIZE - 1 && board[now->row][now->column] == 0)//跑完了
		{


			push(now);
			board[now->row][now->column] = ++step_counter;
			if (show)
			{
				system("cls");
				printf("=============================backtracking mode=============================\n");
				printf("automode:");
				if (mode_number == AUTO)
					printf("on\n");
				else
					printf("off\n");
			}
			printf("第%d次\n", ++print_counter);
			printboard();
			printf("\n");
			
			board[now->row][now->column] = 0;
			//board[before->row][before->column] = 0;//不知道对不对
			
			//refresh_next_board(before, Instruction_ADD);
			//refresh_next_board(now, Instruction_ADD);
			
			step_counter--;
			Re = pop();
			Re->back_track_counter = 0;//复原
			now = get_top();
			if (show && print_counter < solution_upper_bound)
			{
				if (mode_number == MANUAL)
					system("pause");
				else
					wait();
				show_printer();
			}continue;
		}


		if (!board_for_next_step[now->row][now->column])//如果现在没有子节点可以走了
		{
			//board[now->row][now->column] = 0;
			board[before->row][before->column] = 0;
			refresh_next_board(before, Instruction_ADD);
			//refresh_next_board(now, Instruction_ADD);
			step_counter--;
			Re = pop();
			Re->back_track_counter = 0;//复原
			now = get_top();
			if (show)
				show_printer();
			continue;
		}
		
		if (now->back_track_counter >= board_for_next_step[now->row][now->column])//可行方向都搞过了
		{

			

			if (board_for_next_step[now->row][now->column] == 0)//这个说明压根就没有可行方向
			{
				board[before->row][before->column] = 0;
			}

			else
			{
				board[now->row][now->column] = 0;
			}
			//refresh_next_board(before, Instruction_ADD);
			refresh_next_board(now, Instruction_ADD);
			step_counter--;
			Re = pop();
			Re->back_track_counter = 0;//复原
			if (stack.top > -1)//避免最后弹栈完毕之后报错
				now = get_top();
			if (show)
				show_printer();
			continue;
		}
		if (step_counter == board[now->row][now->column])//这个条件说明当前的now结点之前走过了，now是从栈里面出来的
		{
			before = now;
			//refresh_next_board(before, Instruction_SUB);
			now = next_step(before);
			if (no_more_grandson)
			{
				now = before;
				board[now->row][now->column] = 0;
				//refresh_next_board(before, Instruction_ADD);
				refresh_next_board(now, Instruction_ADD);
				step_counter--;
				Re = pop();
				Re->back_track_counter = 0;//复原
				now = get_top();
				if (show)
					show_printer();
				continue;
			}

		}
		else//否则说明当前的now结点是next产生的
		{
			if (check_coordinate(testnode))//如果当前位置合法
			{
				push(now);
				board[now->row][now->column] = ++step_counter;
				before = now;
				refresh_next_board(before, Instruction_SUB);
				now = next_step(before);
				if (no_more_grandson)
				{
					now = before;
					board[now->row][now->column] = 0;
					//refresh_next_board(before, Instruction_ADD);
					refresh_next_board(now, Instruction_ADD);
					step_counter--;
					Re = pop();
					Re->back_track_counter = 0;//复原
					now = get_top();
					if (show)
						show_printer();
					continue;
				}

			}
		}//else
	




		//展示
		if (show)
			show_printer();
		
	}//while

	return;
}





