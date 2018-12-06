#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"head.h"
#include<stdlib.h>

extern int board[15][15];
extern int w;//����
extern int b;//����
extern int coordinate[2];
extern bool show_me_the_array;//������
extern bool ai_first;
extern long int empty_score_total_black[15][15];
extern long int empty_score_total_white[15][15];

extern int  temp_point[2];
//����ʽ�������������ȼ�˳�򣬶Է�֧����������߼�֦�ٶ�
//��https://github.com/lihongxun945/gobang/blob/master/src/ai/board.js

int before_evaluation_ver5(int priority_ver2[][2], int step_count)
{
	long int empty_score_for_priority[15][15] = { 0 };
	int my_raw = 0;
	int my_column = 0;
	long int my_value = 0;
	long int opponent_value = 0;
	long int ** temp_priority;//ָ��Ķ�̬����
	long int * p1;
	long int temp_value = 0;
	int status = 0;
	//0��ʾ��ǰ�������
	//1��ʾ��ǰ��ַ����ҷ����壬�������Է�����
	int array_size = 1;
	int index;
	int temp_raw;
	int temp_column;
	temp_priority = (long int **)malloc(array_size * sizeof(long int*));

	if (step_count % 2)//˵���ǰ��巽���ʴ˺���
	{
		for (my_raw = 0; my_raw < 15; my_raw++)
		{
			for (my_column = 0; my_column < 15; my_column++)
			{
				if ((board[my_raw][my_column] != b)
					&& (board[my_raw][my_column] != w))
				{
					//temp_value = empty_score_total[my_raw][my_column];
					my_value = empty_score_total_white[my_raw][my_column];
					//my_value = labs(my_value);
					opponent_value = empty_score_total_black[my_raw][my_column];
					//opponent_value = labs(opponent_value);
					temp_value = my_value + opponent_value;
					empty_score_for_priority[my_raw][my_column] = temp_value;
					if (temp_value != 0)
					{
						if (my_value >= Consecutive_Five)//�����ҷ����壬����
						{
							status = 1;
							return status;
						}
						
						else//�������¼������
						{//����д������㷨
							temp_priority[array_size - 1] = &empty_score_for_priority[my_raw][my_column];
							array_size++;
							temp_priority = (long int **)realloc(temp_priority, sizeof(long int *)*array_size);
							

						}
					}
				}
			}
		}
	}

	else//˵���Ǻ��巽���ʵĴ˺���
	{
		for (my_raw = 0; my_raw < 15; my_raw++)
		{
			for (my_column = 0; my_column < 15; my_column++)
			{
				if ((board[my_raw][my_column] != b)
					&& (board[my_raw][my_column] != w))
				{
					//temp_value = empty_score_total[my_raw][my_column];
					my_value = empty_score_total_black[my_raw][my_column];
					//my_value = labs(my_value);
					opponent_value = empty_score_total_white[my_raw][my_column];
					//opponent_value = labs(opponent_value);
					temp_value = my_value + opponent_value;
					empty_score_for_priority[my_raw][my_column] = temp_value;
					if (temp_value != 0)
					{
						if (my_value >= Consecutive_Five)//�����ҷ����壬����
						{
							status = 1;
							return status;
						}

						else//�������¼������
						{//����д������㷨
							temp_priority[array_size - 1] = &empty_score_for_priority[my_raw][my_column];
							array_size++;
							temp_priority = (long int **)realloc(temp_priority, sizeof(long int *)*array_size);

						}
					}
				}
			}
		}
	}
	temp_priority[array_size - 1]= 0;


	quick_sort2(temp_priority, 0, array_size);
	p1 = &empty_score_for_priority[0][0];
	for (index = 0; (index < array_size) && (index < 10); index++)//��temp��ǰʮ������긳ֵ�������prioriy_ver2
	{
		temp_raw = (long int)(temp_priority[index] - p1) / 15;
		temp_column = (long int)(temp_priority[index] - p1) % 15;
		priority_ver2[index][0] = temp_raw;
		priority_ver2[index][1] = temp_column;
		/*
		if (temp != *chess_pointer)
		{
			printf(" ");
		}
		*/
	}
	free(temp_priority);
	return status;
}

void refresh_score(int step_count, bool my_turn)//����ˢ�¸����µ��ӵ��ĸ�������6����λ���ڵĿո�ķ���������ٶȹ��������޸�Ϊ4����λ���ڵĿո�
{
	
	int horizon[2] = { 0,1 };//ˮƽ����λʸ��
	int perpendicular[2] = { 1,0 };//��ֱ����λʸ��
	int up_right_down_left[2] = { -1,1 };//�������·���λʸ��
	int up_left_down_right[2] = { 1,1 };//�������·���λʸ��
	/*
	for (i = -6, j = -6; i < 6 && j < 6; i++, j++)
	{
		re_calculate(horizon, i, j, step_count, my_turn);//ˢ��ˮƽ����Ŀո�ķ���
		re_calculate(perpendicular, i, j, step_count, my_turn);//ˢ����ֱ����Ŀո�ķ���
		re_calculate(up_right_down_left, i, j, step_count, my_turn);//ˢ���������·���Ŀո�ķ���
		re_calculate(up_left_down_right, i, j, step_count, my_turn);//ˢ���������·���Ŀո�ķ���

	}
	*/
	re_calculate(horizon, step_count, my_turn);//ˢ��ˮƽ����Ŀո�ķ���
	re_calculate(perpendicular, step_count, my_turn);//ˢ����ֱ����Ŀո�ķ���
	re_calculate(up_right_down_left, step_count, my_turn);//ˢ���������·���Ŀո�ķ���
	re_calculate(up_left_down_right, step_count, my_turn);//ˢ���������·���Ŀո�ķ���
}

void re_calculate(int vector[], int step_count, bool my_turn)
{
	/*
	int raw, column;
	long int value;
	raw = temp_point[0] + vector[0] * i;
	column = temp_point[1] + vector[1] * j;
	value = evaluation_ver2(step_count, my_turn, raw, column);
	empty_score_total[raw][column] = value;
	*/
	int i, j;
	int raw, column, point;
	long int white_value, black_value;
	for (i = -6, j = -6; i < 7 && j < 7; i++, j++)
	{
		raw = temp_point[0] + vector[0] * i;
		column = temp_point[1] + vector[1] * j;
		if (raw >= 0 && raw <= 14 && column >= 0 && column <= 14)
		{
			point = board[raw][column];
			if (point != w && point != b && (raw != temp_point[0] || column != temp_point[1]))
			{//�ж�����Ϊ���������ӣ��Ҳ��Ǹո�������Ǹ���
//֮������Ӻ����Ǹ���������Ϊ�ڳ������Ӳ������÷������ȥ��ʱ�򣬸ո�������Ǹ�λ��Ҳ�ᱻ������һ�飬����ʵ�����Ǹ�λ�ò�����
				black_value = evaluation_ver2(2, true, raw, column);//��step_countǿ����Ϊż��2����֤�Ǻ��ӵ÷֣�����my_turnǿ����Ϊtrue����֤����÷�Ϊ����
				white_value = evaluation_ver2(3, true, raw, column);//��step_countǿ����Ϊ����3����֤�ǰ��ӵ÷֣�����my_turnǿ����Ϊtrue����֤����÷�Ϊ����
				empty_score_total_black[raw][column] = black_value;
				empty_score_total_white[raw][column] = white_value;
				//if (empty_score_total_white[5][10] == 99950)
					//printf("test\n");
			}
		}
		
	}
}


void quick_sort(long int temp_priority[][3], int l, int r)//�Զ�ά������н���Ŀ�������
{//�˺����ο�https://blog.csdn.net/morewindows/article/details/6684558
	if (l < r)
	{

		int i = l, j = r;
		int x[3];
		x[0] = temp_priority[l][0];
		x[1] = temp_priority[l][1];
		x[2] = temp_priority[l][2];
		while (i < j)
		{
			while (i < j && temp_priority[j][0] < x[0])
				j--;
			if (i < j)
			{
				temp_priority[i][0] = temp_priority[j][0];
				temp_priority[i][1] = temp_priority[j][1];
				temp_priority[i][2] = temp_priority[j][2];

				//system("pause");
				i++;
			}


			while (i < j && temp_priority[i][0] >= x[0])
				i++;
			if (i < j)
			{
				temp_priority[j][0] = temp_priority[i][0];
				temp_priority[j][1] = temp_priority[i][1];
				temp_priority[j][2] = temp_priority[i][2];

				//system("pause");
				j--;
			}


		}
		temp_priority[i][0] = x[0];
		temp_priority[i][1] = x[1];
		temp_priority[i][2] = x[2];
		quick_sort(temp_priority, l, i - 1);
		quick_sort(temp_priority, i + 1, r);
	}
}

//��������
void quick_sort2(long int** temp_priority, int l, int r)
{
	if (l < r)
	{
		//Swap(s[l], s[(l + r) / 2]); //���м��������͵�һ�������� �μ�ע1
		int i = l, j = r, x = *temp_priority[l];
		while (i < j)
		{
			while (i < j && *temp_priority[j] >= x) // ���������ҵ�һ��С��x����
				j--;
			if (i < j)
				*temp_priority[i++] = *temp_priority[j];

			while (i < j && *temp_priority[i] < x) // ���������ҵ�һ�����ڵ���x����
				i++;
			if (i < j)
				*temp_priority[j--] = *temp_priority[i];
		}
		*temp_priority[i] = x;
		quick_sort2(temp_priority, l, i - 1); // �ݹ���� 
		quick_sort2(temp_priority, i + 1, r);
	}
}