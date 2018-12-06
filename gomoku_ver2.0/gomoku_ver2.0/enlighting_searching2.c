#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"head.h"
#include<stdlib.h>

extern int board[15][15];
extern int w;//白棋
extern int b;//黑棋
extern int coordinate[2];
extern bool show_me_the_array;//测试用
extern bool ai_first;
extern long int empty_score_total_black[15][15];
extern long int empty_score_total_white[15][15];

extern int  temp_point[2];
//启发式搜索，按照优先级顺序，对分支进行排序，提高剪枝速度
//见https://github.com/lihongxun945/gobang/blob/master/src/ai/board.js

int before_evaluation_ver5(int priority_ver2[][2], int step_count)
{
	long int empty_score_for_priority[15][15] = { 0 };
	int my_raw = 0;
	int my_column = 0;
	long int my_value = 0;
	long int opponent_value = 0;
	long int ** temp_priority;//指针的动态数组
	long int * p1;
	long int temp_value = 0;
	int status = 0;
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五，不包括对方连五
	int array_size = 1;
	int index;
	int temp_raw;
	int temp_column;
	temp_priority = (long int **)malloc(array_size * sizeof(long int*));

	if (step_count % 2)//说明是白棋方访问此函数
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
						if (my_value >= Consecutive_Five)//发现我方连五，返回
						{
							status = 1;
							return status;
						}
						
						else//否则继续录入数据
						{//这里写排序的算法
							temp_priority[array_size - 1] = &empty_score_for_priority[my_raw][my_column];
							array_size++;
							temp_priority = (long int **)realloc(temp_priority, sizeof(long int *)*array_size);
							

						}
					}
				}
			}
		}
	}

	else//说明是黑棋方访问的此函数
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
						if (my_value >= Consecutive_Five)//发现我方连五，返回
						{
							status = 1;
							return status;
						}

						else//否则继续录入数据
						{//这里写排序的算法
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
	for (index = 0; (index < array_size) && (index < 10); index++)//将temp的前十大的坐标赋值给外面的prioriy_ver2
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

void refresh_score(int step_count, bool my_turn)//重新刷新刚落下的子的四个方向上6个单位以内的空格的分数，如果速度过慢，就修改为4个单位以内的空格
{
	
	int horizon[2] = { 0,1 };//水平方向单位矢量
	int perpendicular[2] = { 1,0 };//垂直方向单位矢量
	int up_right_down_left[2] = { -1,1 };//右上左下方向单位矢量
	int up_left_down_right[2] = { 1,1 };//左上右下方向单位矢量
	/*
	for (i = -6, j = -6; i < 6 && j < 6; i++, j++)
	{
		re_calculate(horizon, i, j, step_count, my_turn);//刷新水平方向的空格的分数
		re_calculate(perpendicular, i, j, step_count, my_turn);//刷新竖直方向的空格的分数
		re_calculate(up_right_down_left, i, j, step_count, my_turn);//刷新右上左下方向的空格的分数
		re_calculate(up_left_down_right, i, j, step_count, my_turn);//刷新左上右下方向的空格的分数

	}
	*/
	re_calculate(horizon, step_count, my_turn);//刷新水平方向的空格的分数
	re_calculate(perpendicular, step_count, my_turn);//刷新竖直方向的空格的分数
	re_calculate(up_right_down_left, step_count, my_turn);//刷新右上左下方向的空格的分数
	re_calculate(up_left_down_right, step_count, my_turn);//刷新左上右下方向的空格的分数
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
			{//判断条件为：不是棋子，且不是刚刚下落的那个点
//之所以添加后面那个条件是因为在撤回棋子并重新让分数变回去的时候，刚刚下落的那个位置也会被重新算一遍，但是实际上那个位置不用算
				black_value = evaluation_ver2(2, true, raw, column);//将step_count强行令为偶数2，保证是黑子得分，并将my_turn强行置为true，保证输出得分为正数
				white_value = evaluation_ver2(3, true, raw, column);//将step_count强行令为奇数3，保证是白子得分，并将my_turn强行置为true，保证输出得分为正数
				empty_score_total_black[raw][column] = black_value;
				empty_score_total_white[raw][column] = white_value;
				//if (empty_score_total_white[5][10] == 99950)
					//printf("test\n");
			}
		}
		
	}
}


void quick_sort(long int temp_priority[][3], int l, int r)//对二维数组进行降序的快速排序
{//此函数参考https://blog.csdn.net/morewindows/article/details/6684558
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

//快速排序
void quick_sort2(long int** temp_priority, int l, int r)
{
	if (l < r)
	{
		//Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
		int i = l, j = r, x = *temp_priority[l];
		while (i < j)
		{
			while (i < j && *temp_priority[j] >= x) // 从右向左找第一个小于x的数
				j--;
			if (i < j)
				*temp_priority[i++] = *temp_priority[j];

			while (i < j && *temp_priority[i] < x) // 从左向右找第一个大于等于x的数
				i++;
			if (i < j)
				*temp_priority[j--] = *temp_priority[i];
		}
		*temp_priority[i] = x;
		quick_sort2(temp_priority, l, i - 1); // 递归调用 
		quick_sort2(temp_priority, i + 1, r);
	}
}