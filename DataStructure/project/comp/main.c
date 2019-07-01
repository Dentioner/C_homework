#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define BOARD_SIZE 64
#define Instruction_ADD 1
#define Instruction_SUB 2



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

bool no_more_grandson = false;//next_step��

SqStack stack;
Node *now, *before;

Node nodeboard[8][8];
double start_time_all = 0;
double end_time_all = 0;
int main()
{
	double procedure_time;
	Init_nodeboard();
	Init_Stack();
	int first_row, first_column;
	printf("Please choose a start TreeNode. Use ',' to distinguish row & column.\n");
	scanf("%d,%d", &(first_row), &(first_column));
	printf("Step by step? Enter 1 means yes and 0 means no.\n");
	scanf("%d", &show);
	now = &nodeboard[first_row][first_column];

	start_time_all = clock();//test
	horse();
	
    printboard();
	end_time_all = clock();//test
	procedure_time = end_time_all - start_time_all;
	printf("\ntime = %f\n", procedure_time);
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

			//������debug�õ�
			
			if (board[1][5] == 0 || board[2][6] == 0)
			{
				if (board_for_next_step[0][7] <= 0)
					printf("test.\n");
			}
			
			if (board[1][5] == 0 && board[2][6] == 0)
			{
				if (board_for_next_step[0][7] <= 1)
					printf("test.\n");
			}

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
	
	while (stack.top!=-1&& print_counter < 10  )//ֻҪջ��Ϊ�գ���һֱ��������ȥ
	{
		//now = get_top();
		/*
		if (step_counter == 63)
		{
			printf("test.\n");
			printboard();
		}
		*/

		testnode.row = now->row;
		testnode.column = now->column;
		//if (step_counter == BOARD_SIZE)
		//{
			
			
		//}
		if (step_counter == BOARD_SIZE - 1 && board[now->row][now->column] == 0)//������
		{
			if (print_counter == 1375)
				printf("test\n");//test

			push(now);
			board[now->row][now->column] = ++step_counter;

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
			continue;
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
			continue;
		}
		
		if (now->back_track_counter >= board_for_next_step[now->row][now->column])//���з��򶼸����
		{

			if (step_counter == 7 && print_counter == 1432)
				printf("test.\n");

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
					continue;
				}

			}
		}
	}




		//������չʾ�õģ�����ɾ��
		if (show)
		{
			system("cls");
			printboard();
			system("pause");
		}
	

	return;
}





