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
#define TIME_LIMIT 1000 //��λ��ms


typedef struct TNode
{
	bool banned;//��������ڵ��߲�ͨ��
	int row;
	int column;
	struct TNode * children[8];//8������
	struct TNode * parent;
}TreeNode;

typedef struct Node
{
	int row;
	int column;
	int son[8][2];//��¼��������,ÿ��son[i][0]��¼��������ڴ˽ڵ�ķ�λ����vector���������ֵ
	//ÿ��son[i][1]��¼���Ӻ��е����Ӹ���
	//���磬�����Ѷ�����λ�ڱ��ڵ���ұߵ����·��Ǹ�4�Žڵ㣬�ýڵ��2�����ӣ���ôs[0][1]=4��s[0][2] = 2;
	//���ĳ�������߲�ͨ����ô�������Ӹ���Ϊ9���ϣ��Ա������ʱ���ܹ����������
	int back_track_counter;//��¼���ݴ�����ͬʱ�˱���Ҳ����Ϊ��һ�����ʵĶ��������Ԫ������
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
int board_for_next_step[8][8] = {//�������������¼ĳ������һ�������ߵķ�λ�м���
	{2, 3, 4, 4, 4, 4, 3, 2},//ÿ��һ���Ӿ͵ø���һ����
	{3, 4, 6, 6, 6, 6, 4, 3},//������������ἰ����������
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

int show;//չʾ��
int mode_number;
int solution_upper_bound;

bool no_more_grandson = false;//next_step��

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

void refresh_next_board(Node* point, int instruction)//ˢ����������
{//instruction ָ���ǣ���Ҫ��������Ҫ�ӣ���Ϊ�����ʱ��ûָ����Ӹ���
	Node testnode;
	Node* next;//�����nextʵ��������nowΪ��һ������Щ��
	for (int vector_index = 0; vector_index < 8; vector_index++)//8���������
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

bool check_coordinate(Node testpoint)//����Ƿ��ǺϷ�����
{
	if (testpoint.row >= 8 || testpoint.row < 0 || testpoint.column >= 8 || testpoint.column < 0
		|| board[testpoint.row][testpoint.column])//������̵��Ǹ�λ�ò��Ϸ������߲���0
		return false;
	else
		return true;
}

Node* next_step(Node* point)//����һ��Ӧ��ȥ��
{//ʹ��̰���㷨��������������
	//�ڴ˲�������ÿ��Node�Ķ�������
	Node testnode;
	Node* next;
	Node* best_son;
	bool has_been_initialized = false;//��Ѷ����Ƿ񱻳�ʼ����
	int i = 0;
	int vector_index = 0;
	//���������ɶ���֮ǰ�ȿ�����û��֮ǰ���ɹ������ˣ�����Ͳ�������һ��ѭ��
	//�ж�֮ǰ��û�����ɶ��ӵķ�ʽ���Ҿ��ÿ��Ը���Node�ṹ������Ǹ����ݼ���������

	if (point->row == 2 && point->column == 1 && step_counter == 2)
		printf("\n");


	if (!point->back_track_counter)//������Ϊ0˵��û�л��ݹ�
	{
		for (; vector_index < 8; vector_index++)//8���������
		{
			testnode.row = point->row + vector[vector_index][0];
			testnode.column = point->column + vector[vector_index][1];
			next = &nodeboard[point->row + vector[vector_index][0]][point->column + vector[vector_index][1]];
			point->son[i][0] = vector_index;


			if (!check_coordinate(testnode))
			{
				point->son[i++][1] = 9 + vector_index;//Ϲд��һ������9����ֵ			
				continue;
			}
			else
			{
				point->son[i++][1] = board_for_next_step[next->row][next->column];
			}
		}
		//����8��֮��ִ������

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

		}//����
		
		//******����ط��ƺ�û�м�����ɵ���һ��������ֵ���ǲ���������
		best_son = 
			&nodeboard
			[point->row + vector[point->son[point->back_track_counter][0]][0]]
			[point->column + vector[point->son[point->back_track_counter][0]][1]];
		
		if (point->son[point->back_track_counter][1] > 8)//˵��û�и���������ˣ�����8�Ķ���֮ǰԤ�������ֵ
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
		//******����ط��ƺ�û�м�����ɵ���һ��������ֵ���ǲ���������
		best_son =
			&nodeboard
			[point->row + vector[point->son[point->back_track_counter][0]][0]]
			[point->column + vector[point->son[point->back_track_counter][0]][1]];

		if (point->son[point->back_track_counter][1] > 8)//˵��û�и���������ˣ�����8�Ķ���֮ǰԤ�������ֵ
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
	printf("��%d��\n", print_counter + 1);
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
	Node* Re;//���ڴ洢pop�ķ���ֵ
	if (!step_counter)//��һ������ѡ���ĵط�д��1
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

	while (stack.top!=-1 && print_counter < solution_upper_bound)//ֻҪջ��Ϊ�գ���һֱ��������ȥ
	{

		testnode.row = now->row;
		testnode.column = now->column;

		if (step_counter == BOARD_SIZE - 1 && board[now->row][now->column] == 0)//������
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
			printf("��%d��\n", ++print_counter);
			printboard();
			printf("\n");
			
			board[now->row][now->column] = 0;
			//board[before->row][before->column] = 0;//��֪���Բ���
			
			//refresh_next_board(before, Instruction_ADD);
			//refresh_next_board(now, Instruction_ADD);
			
			step_counter--;
			Re = pop();
			Re->back_track_counter = 0;//��ԭ
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


		if (!board_for_next_step[now->row][now->column])//�������û���ӽڵ��������
		{
			//board[now->row][now->column] = 0;
			board[before->row][before->column] = 0;
			refresh_next_board(before, Instruction_ADD);
			//refresh_next_board(now, Instruction_ADD);
			step_counter--;
			Re = pop();
			Re->back_track_counter = 0;//��ԭ
			now = get_top();
			if (show)
				show_printer();
			continue;
		}
		
		if (now->back_track_counter >= board_for_next_step[now->row][now->column])//���з��򶼸����
		{

			

			if (board_for_next_step[now->row][now->column] == 0)//���˵��ѹ����û�п��з���
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
			Re->back_track_counter = 0;//��ԭ
			if (stack.top > -1)//�������ջ���֮�󱨴�
				now = get_top();
			if (show)
				show_printer();
			continue;
		}
		if (step_counter == board[now->row][now->column])//�������˵����ǰ��now���֮ǰ�߹��ˣ�now�Ǵ�ջ���������
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
				Re->back_track_counter = 0;//��ԭ
				now = get_top();
				if (show)
					show_printer();
				continue;
			}

		}
		else//����˵����ǰ��now�����next������
		{
			if (check_coordinate(testnode))//�����ǰλ�úϷ�
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
					Re->back_track_counter = 0;//��ԭ
					now = get_top();
					if (show)
						show_printer();
					continue;
				}

			}
		}//else
	




		//չʾ
		if (show)
			show_printer();
		
	}//while

	return;
}





