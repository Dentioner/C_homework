#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"head.h"

extern int board[15][15];
extern int w;//白棋
extern int b;//黑棋

//启发式搜索，按照优先级顺序，对分支进行排序，提高剪枝速度
//见https://github.com/lihongxun945/gobang/blob/master/src/ai/board.js

int before_evaluation_ver4(int priority_ver2[][2], int floor, int step_count, bool my_turn)
{
	//char * temp = *chess_pointer;
	long int temp_priority[255][3] = { 0 };//先将255个点的评分全部存下来
	int my_raw = 0;
	int my_column = 0;

	int index = 0;
	int i = 0;
	long int my_value = 0;
	long int opponent_value = 0;
	long int temp_value = 0;

	int status = 0;
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五，不包括对方连五

	for (my_raw = 0; my_raw < 15; my_raw++)
	{
		for (my_column = 0; my_column < 15; my_column++)
		{
			if ((board[my_raw][my_column] != b)
				&& (board[my_raw][my_column] != w))
			{
				/*
				if (temp != *chess_pointer)
				{
					printf(" ");
				}
				*/

				my_value = evaluation_ver2(step_count, my_turn, my_raw, my_column);
				my_value = labs(my_value);
				opponent_value = evaluation_ver2(step_count + 1, !my_turn, my_raw, my_column);
				opponent_value = labs(opponent_value);
				temp_value = my_value + opponent_value;
				if (temp_value != 0)
				{
					if (my_value >= Consecutive_Five)//发现我方连五，返回
					{
						status = 1;
						return status;
					}
					/*
					else if (opponent_value >= Consecutive_Five)//发现对方连五，返回
					{
						status = 1;
						return status;
					}
					*/
					else//否则继续录入数据
					{


						temp_priority[index][0] = temp_value;
						temp_priority[index][1] = my_raw;
						temp_priority[index][2] = my_column;
						index++;
					}
				}
			}
		}
	}

	//测试，打印temp数组
	/*
	for (int ceshi = 0; ceshi < 255; ceshi++)
	{
		printf("评分：%d，坐标：(%d, %d)\n", temp_priority[ceshi][0], temp_priority[ceshi][1], temp_priority[ceshi][2]);
	}
	system("pause");
	*/

	quick_sort(temp_priority, 0, index - 1);//index的值是第一个0元素的位置
	//测试，打印temp数组

	if (show_me_the_array)
	{

		for (int ceshi = 0; ceshi < 10; ceshi++)
		{
			printf("评分：%d，坐标：(%d, %d)\n", temp_priority[ceshi][0], temp_priority[ceshi][1], temp_priority[ceshi][2]);
		}
		//system("pause");
	}


	for (i = 0; (i < index) && (i < 10); i++)//将temp的前十大的坐标赋值给外面的prioriy_ver2
	{
		priority_ver2[i][0] = temp_priority[i][1];
		priority_ver2[i][1] = temp_priority[i][2];
		/*
		if (temp != *chess_pointer)
		{
			printf(" ");
		}
		*/
	}

	//测试，打印priority数组
	/*
	for (int ceshi = 0; ceshi < 10; ceshi++)
	{
		printf("坐标：(%d, %d)\n", priority_ver2[ceshi][0], priority_ver2[ceshi][1]);
	}
	system("pause");
	*/
	/*
	if (temp != *chess_pointer)
	{
		printf(" ");
	}
	*/
	return status;
}




void quick_sort(int temp_priority[][3], int l, int r)//对二维数组进行降序的快速排序
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