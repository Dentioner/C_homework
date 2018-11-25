#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"head.h"


//启发式搜索，按照优先级顺序，对分支进行排序，提高剪枝速度
//见https://github.com/lihongxun945/gobang/blob/master/src/ai/board.js
bool before_evaluation(char board[][17][3], int priority[][30][2], int floor,
	int step_count, bool my_turn)
//step_count及其后面的几个形参是用来给evaluation的
{
	int my_raw = 0;
	int my_column = 0;
	char black[2] = "○";
	char white[2] = "●";
	long int my_value = 0;
	long int opponent_value = 0;
	bool find_five = false;
	bool find_opponent_five = false;
	bool find_OpenFour1 = false;
	bool find_OpenFour2 = false;
	bool find_OpenFour3 = false;
	bool find_opponent_OpenFour1 = false;
	bool find_opponent_OpenFour2 = false;
	bool find_opponent_OpenFour3 = false;
	bool find_DoubleThree1 = false;
	bool find_DoubleThree2 = false;
	bool find_DoubleThree3 = false;
	bool find_opponent_DoubleThree1 = false;
	bool find_opponent_DoubleThree2 = false;
	bool find_opponent_DoubleThree3 = false;
	bool find_CapFour1 = false;
	bool find_CapFour2 = false;
	bool find_CapFour3 = false;
	bool find_opponent_CapFour1 = false;
	bool find_opponent_CapFour2 = false;
	bool find_opponent_CapFour3 = false;
	bool find_three1 = false;
	bool find_three2 = false;
	bool find_three3 = false;
	bool find_opponent_three1 = false;
	bool find_opponent_three2 = false;
	bool find_opponent_three3 = false;
	bool final_hit = false;//发现我方连五直接返回
	//先初始化，将之前别的分支用过的记录归为0

	for (int ele = 0; ele < 26; ele++)
	{
		priority[FLOOR - floor][ele][0] = 0;
		priority[FLOOR - floor][ele][1] = 0;
	}
	//下面开始正式记录
	for (my_raw = 0; my_raw < 15; my_raw++)
	{
		for (my_column = 1; my_column < 16; my_column++)
		{
			if ((strncmp(board[my_raw][my_column], black, 2) != 0)
				&& (strncmp(board[my_raw][my_column], white, 2) != 0))
			{
				my_value = evaluation(board, step_count, my_turn, my_raw, my_column);
				my_value = labs(my_value);
				opponent_value = evaluation(board, step_count + 1, !my_turn, my_raw, my_column);
				opponent_value = labs(opponent_value);
				if (my_value >= Consecutive_Five)//如果发现了我方连五的地方
				{
					priority[FLOOR - floor][0][0] = my_raw;
					priority[FLOOR - floor][0][1] = my_column;
					find_five = true;
					final_hit = true;
					return final_hit;
				}
				else if (opponent_value >= Consecutive_Five)//如果发现了对方连五的地方
				{
					priority[FLOOR - floor][1][0] = my_raw;
					priority[FLOOR - floor][1][1] = my_column;
					find_opponent_five = true;
				}
				else if (my_value >= Open_Four)
				{
					
					if (!find_OpenFour1)//如果第一个活四的位置是空的
					{//第3,4,5属于三个活四的坐标
						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						find_OpenFour1 = true;
					}
					else if (!find_OpenFour2)//如果第2个活四的位置是空的
					{
						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						find_OpenFour2 = true;
					}
					else if (!find_OpenFour3)//如果第3个活四的位置是空的
					{
						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						find_OpenFour3 = true;
					}
				}
				else if (opponent_value >= Open_Four)
				{
					if (!find_opponent_OpenFour1)//如果第一个活四的位置是空的
					{//第6,7,8属于三个活四的坐标，也就是567号位
						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						find_opponent_OpenFour1 = true;
					}
					else if (!find_opponent_OpenFour2)//如果第2个活四的位置是空的
					{
						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						find_opponent_OpenFour2 = true;
					}
					else if (!find_opponent_OpenFour3)//如果第3个活四的位置是空的
					{
						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						find_opponent_OpenFour3 = true;
					}


				}
				else if (my_value >= 2 * Gapped_Three)
				{
					if (!find_DoubleThree1)//如果第一个双活三的位置是空的
					{//第9，10，11属于三个双活三的坐标，也就是8,9,10号位
						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						find_DoubleThree1 = true;
					}
					else if (!find_DoubleThree2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						find_DoubleThree2 = true;
					}
					else if (!find_DoubleThree3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][10][0] = my_raw;
						priority[FLOOR - floor][10][1] = my_column;
						find_DoubleThree3 = true;
					}
				}
				else if (opponent_value >= 2 * Gapped_Three)
				{
					if (!find_opponent_DoubleThree1)//如果第一个双活三的位置是空的
					{//第12，13，14属于三个双活三的坐标，也就是11,12,13号位
						priority[FLOOR - floor][11][0] = my_raw;
						priority[FLOOR - floor][11][1] = my_column;
						find_opponent_DoubleThree1 = true;
					}
					else if (!find_opponent_DoubleThree2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][12][0] = my_raw;
						priority[FLOOR - floor][12][1] = my_column;
						find_opponent_DoubleThree2 = true;
					}
					else if (!find_opponent_DoubleThree3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][13][0] = my_raw;
						priority[FLOOR - floor][13][1] = my_column;
						find_opponent_DoubleThree3 = true;
					}
				}
				else if (my_value >= Capped_Four)
				{
					if (!find_CapFour1)//如果第一个冲四的位置是空的
					{//第15，16，17属于三个冲四的坐标，也就是14,15,16号位
						priority[FLOOR - floor][14][0] = my_raw;
						priority[FLOOR - floor][14][1] = my_column;
						find_CapFour1 = true;
					}
					else if (!find_CapFour2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][15][0] = my_raw;
						priority[FLOOR - floor][15][1] = my_column;
						find_CapFour2 = true;
					}
					else if (!find_CapFour3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][16][0] = my_raw;
						priority[FLOOR - floor][16][1] = my_column;
						find_CapFour3 = true;
					}
				}
				else if (opponent_value >= Capped_Four)
				{
					if (!find_opponent_CapFour1)//如果第一个冲四的位置是空的
					{//第18，19，20属于三个冲四的坐标，也就是17,18,19号位
						priority[FLOOR - floor][17][0] = my_raw;
						priority[FLOOR - floor][17][1] = my_column;
						find_opponent_CapFour1 = true;
					}
					else if (!find_opponent_CapFour2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][18][0] = my_raw;
						priority[FLOOR - floor][18][1] = my_column;
						find_opponent_CapFour2 = true;
					}
					else if (!find_opponent_CapFour3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][19][0] = my_raw;
						priority[FLOOR - floor][19][1] = my_column;
						find_opponent_CapFour3 = true;
					}
				}
				else if (my_value >= Gapped_Three)//跳活三和活三放一块了
				{
					if (!find_three1)//如果第一个活三的位置是空的
					{//第21，22，23属于三个活三的坐标，也就是20,21,22号位
						priority[FLOOR - floor][20][0] = my_raw;
						priority[FLOOR - floor][20][1] = my_column;
						find_three1 = true;
					}
					else if (!find_three2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][21][0] = my_raw;
						priority[FLOOR - floor][21][1] = my_column;
						find_three2 = true;
					}
					else if (!find_three3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][22][0] = my_raw;
						priority[FLOOR - floor][22][1] = my_column;
						find_three3 = true;
					}
				}
				else if (opponent_value >= Gapped_Three)
				{
					if (!find_opponent_three1)//如果第一个活三的位置是空的
					{//第24，25，26属于三个活三的坐标，也就是23,24,25号位
						priority[FLOOR - floor][23][0] = my_raw;
						priority[FLOOR - floor][23][1] = my_column;
						find_opponent_three1 = true;
					}
					else if (!find_opponent_three2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][24][0] = my_raw;
						priority[FLOOR - floor][24][1] = my_column;
						find_opponent_three2 = true;
					}
					else if (!find_opponent_three3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][25][0] = my_raw;
						priority[FLOOR - floor][25][1] = my_column;
						find_opponent_three3 = true;
					}
				}

			}

			if (find_five
				&& find_opponent_five
				&& find_OpenFour1
				&& find_OpenFour2
				&& find_OpenFour3
				&& find_opponent_OpenFour1
				&& find_opponent_OpenFour2
				&& find_opponent_OpenFour3
				&& find_DoubleThree1
				&& find_DoubleThree2
				&& find_DoubleThree3
				&& find_opponent_DoubleThree1
				&& find_opponent_DoubleThree2
				&& find_opponent_DoubleThree3
				&& find_CapFour1
				&& find_CapFour2
				&& find_CapFour3
				&& find_opponent_CapFour1
				&& find_opponent_CapFour2
				&& find_opponent_CapFour3
				&& find_opponent_three1
				&& find_opponent_three2
				&& find_opponent_three3
				&& find_three1
				&& find_three2
				&& find_three3)
			{
				return final_hit;
			}

		}




	}
	
	return final_hit;
}



int before_evaluation_ver2(char board[][17][3], int priority[][30][2], int floor,
	int step_count, bool my_turn)
	//step_count及其后面的几个形参是用来给evaluation的
{
	int my_raw = 0;
	int my_column = 0;
	char black[2] = "○";
	char white[2] = "●";
	long int my_value = 0;
	long int opponent_value = 0;
	bool find_five = false;
	bool find_opponent_five = false;
	bool find_OpenFour1 = false;
	bool find_OpenFour2 = false;
	bool find_OpenFour3 = false;
	bool find_opponent_OpenFour1 = false;
	bool find_opponent_OpenFour2 = false;
	bool find_opponent_OpenFour3 = false;
	bool find_DoubleThree1 = false;
	bool find_DoubleThree2 = false;
	bool find_DoubleThree3 = false;
	bool find_opponent_DoubleThree1 = false;
	bool find_opponent_DoubleThree2 = false;
	bool find_opponent_DoubleThree3 = false;
	bool find_CapFour1 = false;
	bool find_CapFour2 = false;
	bool find_CapFour3 = false;
	bool find_opponent_CapFour1 = false;
	bool find_opponent_CapFour2 = false;
	bool find_opponent_CapFour3 = false;
	bool find_three1 = false;
	bool find_three2 = false;
	bool find_three3 = false;
	bool find_opponent_three1 = false;
	bool find_opponent_three2 = false;
	bool find_opponent_three3 = false;
	bool find_Capped_three1 = false;
	bool find_Capped_three2 = false;
	bool find_opponent_Capped_three1 = false;
	bool find_opponent_Capped_three2 = false;
	
	int status = 0;//发现我方连五直接返回
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五
	//2表示当前棋局没有发现我方连五，但是发现对方连五



	//先初始化，将之前别的分支用过的记录归为0

	for (int ele = 0; ele < 26; ele++)
	{
		priority[FLOOR - floor][ele][0] = 0;
		priority[FLOOR - floor][ele][1] = 0;
	}
	//下面开始正式记录
	for (my_raw = 0; my_raw < 15; my_raw++)
	{
		for (my_column = 1; my_column < 16; my_column++)
		{
			if ((strncmp(board[my_raw][my_column], black, 2) != 0)
				&& (strncmp(board[my_raw][my_column], white, 2) != 0))
			{
				my_value = evaluation(board, step_count, my_turn, my_raw, my_column);
				my_value = labs(my_value);
				opponent_value = evaluation(board, step_count + 1, !my_turn, my_raw, my_column);
				opponent_value = labs(opponent_value);
				if (my_value >= Consecutive_Five)//如果发现了我方连五的地方
				{
					priority[FLOOR - floor][0][0] = my_raw;
					priority[FLOOR - floor][0][1] = my_column;
					find_five = true;
					status = 1;
					printf("进入了status = %d的情况\n", status);
					system("pause");
					return status;
				}
				else if (opponent_value >= Consecutive_Five)//如果发现了对方连五的地方
				{
					priority[FLOOR - floor][1][0] = my_raw;
					priority[FLOOR - floor][1][1] = my_column;
					find_opponent_five = true;
					
				}
				else if (my_value >= Open_Four)
				{

					if (!find_OpenFour1)//如果第一个活四的位置是空的
					{//第3,4,5属于三个活四的坐标
						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						find_OpenFour1 = true;
					}
					else if (!find_OpenFour2)//如果第2个活四的位置是空的
					{
						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						find_OpenFour2 = true;
					}
					else if (!find_OpenFour3)//如果第3个活四的位置是空的
					{
						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						find_OpenFour3 = true;
					}
				}
				else if (opponent_value >= Open_Four)
				{
					if (!find_opponent_OpenFour1)//如果第一个活四的位置是空的
					{//第6,7,8属于三个活四的坐标，也就是567号位
						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						find_opponent_OpenFour1 = true;
					}
					else if (!find_opponent_OpenFour2)//如果第2个活四的位置是空的
					{
						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						find_opponent_OpenFour2 = true;
					}
					else if (!find_opponent_OpenFour3)//如果第3个活四的位置是空的
					{
						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						find_opponent_OpenFour3 = true;
					}


				}
				else if (my_value >= 2 * Gapped_Three)
				{
					if (!find_DoubleThree1)//如果第一个双活三的位置是空的
					{//第9，10，11属于三个双活三的坐标，也就是8,9,10号位
						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						find_DoubleThree1 = true;
					}
					else if (!find_DoubleThree2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						find_DoubleThree2 = true;
					}
					else if (!find_DoubleThree3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][10][0] = my_raw;
						priority[FLOOR - floor][10][1] = my_column;
						find_DoubleThree3 = true;
					}
				}
				else if (opponent_value >= 2 * Gapped_Three)
				{
					if (!find_opponent_DoubleThree1)//如果第一个双活三的位置是空的
					{//第12，13，14属于三个双活三的坐标，也就是11,12,13号位
						priority[FLOOR - floor][11][0] = my_raw;
						priority[FLOOR - floor][11][1] = my_column;
						find_opponent_DoubleThree1 = true;
					}
					else if (!find_opponent_DoubleThree2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][12][0] = my_raw;
						priority[FLOOR - floor][12][1] = my_column;
						find_opponent_DoubleThree2 = true;
					}
					else if (!find_opponent_DoubleThree3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][13][0] = my_raw;
						priority[FLOOR - floor][13][1] = my_column;
						find_opponent_DoubleThree3 = true;
					}
				}
				else if (my_value >= Capped_Four)
				{
					if (!find_CapFour1)//如果第一个冲四的位置是空的
					{//第15，16，17属于三个冲四的坐标，也就是14,15,16号位
						priority[FLOOR - floor][14][0] = my_raw;
						priority[FLOOR - floor][14][1] = my_column;
						find_CapFour1 = true;
					}
					else if (!find_CapFour2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][15][0] = my_raw;
						priority[FLOOR - floor][15][1] = my_column;
						find_CapFour2 = true;
					}
					else if (!find_CapFour3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][16][0] = my_raw;
						priority[FLOOR - floor][16][1] = my_column;
						find_CapFour3 = true;
					}
				}
				else if (opponent_value >= Capped_Four)
				{
					if (!find_opponent_CapFour1)//如果第一个冲四的位置是空的
					{//第18，19，20属于三个冲四的坐标，也就是17,18,19号位
						priority[FLOOR - floor][17][0] = my_raw;
						priority[FLOOR - floor][17][1] = my_column;
						find_opponent_CapFour1 = true;
					}
					else if (!find_opponent_CapFour2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][18][0] = my_raw;
						priority[FLOOR - floor][18][1] = my_column;
						find_opponent_CapFour2 = true;
					}
					else if (!find_opponent_CapFour3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][19][0] = my_raw;
						priority[FLOOR - floor][19][1] = my_column;
						find_opponent_CapFour3 = true;
					}
				}
				else if (my_value >= Gapped_Three)
				{
					if (!find_three1)//如果第一个活三的位置是空的
					{//第21，22，23属于三个活三的坐标，也就是20,21,22号位
						priority[FLOOR - floor][20][0] = my_raw;
						priority[FLOOR - floor][20][1] = my_column;
						find_three1 = true;
					}
					else if (!find_three2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][21][0] = my_raw;
						priority[FLOOR - floor][21][1] = my_column;
						find_three2 = true;
					}
					else if (!find_three3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][22][0] = my_raw;
						priority[FLOOR - floor][22][1] = my_column;
						find_three3 = true;
					}
				}
				else if (opponent_value >= Gapped_Three)
				{
					if (!find_opponent_three1)//如果第一个活三的位置是空的
					{//第24，25，26属于三个活三的坐标，也就是23,24,25号位
						priority[FLOOR - floor][23][0] = my_raw;
						priority[FLOOR - floor][23][1] = my_column;
						find_opponent_three1 = true;
					}
					else if (!find_opponent_three2)//如果第2个的位置是空的
					{
						priority[FLOOR - floor][24][0] = my_raw;
						priority[FLOOR - floor][24][1] = my_column;
						find_opponent_three2 = true;
					}
					else if (!find_opponent_three3)//如果第3个的位置是空的
					{
						priority[FLOOR - floor][25][0] = my_raw;
						priority[FLOOR - floor][25][1] = my_column;
						find_opponent_three3 = true;
					}
				}

				else if (my_value >= Capped_Three)
				{
					if (!find_Capped_three1)
					{
						priority[FLOOR - floor][26][0] = my_raw;
						priority[FLOOR - floor][26][1] = my_column;
						find_Capped_three1 = true;
					}
					else if (!find_Capped_three2)
					{
						priority[FLOOR - floor][27][0] = my_raw;
						priority[FLOOR - floor][27][1] = my_column;
						find_Capped_three2 = true;
					}
				}
				else if (opponent_value >= Capped_Three)
				{
					if (!find_opponent_Capped_three1)
					{
						priority[FLOOR - floor][28][0] = my_raw;
						priority[FLOOR - floor][28][1] = my_column;
						find_opponent_Capped_three1 = true;
					}
					else if (!find_opponent_Capped_three2)
					{
						priority[FLOOR - floor][29][0] = my_raw;
						priority[FLOOR - floor][29][1] = my_column;
						find_opponent_Capped_three2 = true;
					}
				}

			}

			if (find_five
				&& find_opponent_five
				&& find_OpenFour1
				&& find_OpenFour2
				&& find_OpenFour3
				&& find_opponent_OpenFour1
				&& find_opponent_OpenFour2
				&& find_opponent_OpenFour3
				&& find_DoubleThree1
				&& find_DoubleThree2
				&& find_DoubleThree3
				&& find_opponent_DoubleThree1
				&& find_opponent_DoubleThree2
				&& find_opponent_DoubleThree3
				&& find_CapFour1
				&& find_CapFour2
				&& find_CapFour3
				&& find_opponent_CapFour1
				&& find_opponent_CapFour2
				&& find_opponent_CapFour3
				&& find_opponent_three1
				&& find_opponent_three2
				&& find_opponent_three3
				&& find_three1
				&& find_three2
				&& find_three3
				&& find_Capped_three1
				&& find_Capped_three2
				&& find_opponent_Capped_three1
				&& find_opponent_Capped_three2)
			{
				//printf("进入了status = %d的情况\n", status);
				//system("pause");
				return status;
			}

		}




	}
	if (!find_five&&find_opponent_five)
	{
		
		status = 2;
		//printf("进入了status = %d的情况\n", status);
		//system("pause");
	}
	return status;
}


int before_evaluation_ver3(char board[][17][3], int priority_ver2[][2], int floor,
	int step_count, bool my_turn, char **chess_pointer)
{
	//char * temp = *chess_pointer;
	long int temp_priority[255][3] = { 0 };//先将255个点的评分全部存下来
	int my_raw = 0;
	int my_column = 0;
	char black[2] = "○";
	char white[2] = "●";
	int index = 0;
	int i = 0;
	long int my_value = 0;
	long int opponent_value = 0;
	long int temp_value = 0;
	
	int status = 0;
//0表示当前棋局正常
//1表示当前棋局发现连五，不分敌我

	for (my_raw = 0; my_raw < 15; my_raw++)
	{
		for (my_column = 1; my_column < 16; my_column++)
		{
			if ((strncmp(board[my_raw][my_column], black, 2) != 0)
				&& (strncmp(board[my_raw][my_column], white, 2) != 0))
			{
				/*
				if (temp != *chess_pointer)
				{
					printf(" ");
				}
				*/
				my_value = evaluation(board, step_count, my_turn, my_raw, my_column);
				my_value = labs(my_value);
				opponent_value = evaluation(board, step_count + 1, !my_turn, my_raw, my_column);
				opponent_value = labs(opponent_value);
				temp_value = my_value*1.5 + opponent_value*0.75;
				if (temp_value != 0)
				{	
					if (my_value >= Consecutive_Five)//发现我方连五，返回
					{
						status = 1;
						return status;
					}
					else if (opponent_value >= Consecutive_Five)//发现对方连五，返回
					{
						status = 1;
						return status;
					}
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
	/*
	for (int ceshi = 0; ceshi < 255; ceshi++)
	{
		printf("评分：%d，坐标：(%d, %d)\n", temp_priority[ceshi][0], temp_priority[ceshi][1], temp_priority[ceshi][2]);
	}
	system("pause");
	*/


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
		//Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
		int i = l, j = r;
		int x[3];
		x[0] = temp_priority[l][0];
		x[1] = temp_priority[l][1];
		x[2] = temp_priority[l][2];
		while (i < j)
		{
			while (i < j && temp_priority[j][0] < x[0]) // 从右向左找第一个小于x的数
				j--;
			if (i < j)
			{
				temp_priority[i][0] = temp_priority[j][0];
				temp_priority[i][1] = temp_priority[j][1];
				temp_priority[i][2] = temp_priority[j][2];
				
				//system("pause");
				i++;
			}


			while (i < j && temp_priority[i][0] >= x[0]) // 从左向右找第一个大于等于x的数
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
		quick_sort(temp_priority, l, i - 1); // 递归调用 
		quick_sort(temp_priority, i + 1, r);
	}
}


int before_evaluation_ver4(char board[][17][3], int priority_ver2[][2], int floor,
	int step_count, bool my_turn)
{
	//char * temp = *chess_pointer;
	long int temp_priority[255][3] = { 0 };//先将255个点的评分全部存下来
	int my_raw = 0;
	int my_column = 0;
	char black[2] = "○";
	char white[2] = "●";
	int index = 0;
	int i = 0;
	long int my_value = 0;
	long int opponent_value = 0;
	long int temp_value = 0;

	int status = 0;
	//0表示当前棋局正常
	//1表示当前棋局发现连五，不分敌我

	for (my_raw = 0; my_raw < 15; my_raw++)
	{
		for (my_column = 1; my_column < 16; my_column++)
		{
			if ((strncmp(board[my_raw][my_column], black, 2) != 0)
				&& (strncmp(board[my_raw][my_column], white, 2) != 0))
			{
				/*
				if (temp != *chess_pointer)
				{
					printf(" ");
				}
				*/
				my_value = evaluation(board, step_count, my_turn, my_raw, my_column);
				my_value = labs(my_value);
				opponent_value = evaluation(board, step_count + 1, !my_turn, my_raw, my_column);
				opponent_value = labs(opponent_value);
				temp_value = my_value * 1.5 + opponent_value * 0.75;
				if (temp_value != 0)
				{
					if (my_value >= Consecutive_Five)//发现我方连五，返回
					{
						status = 1;
						return status;
					}
					else if (opponent_value >= Consecutive_Five)//发现对方连五，返回
					{
						status = 1;
						return status;
					}
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
	/*
	for (int ceshi = 0; ceshi < 255; ceshi++)
	{
		printf("评分：%d，坐标：(%d, %d)\n", temp_priority[ceshi][0], temp_priority[ceshi][1], temp_priority[ceshi][2]);
	}
	system("pause");
	*/


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