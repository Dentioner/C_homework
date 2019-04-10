#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include"head.h"
#include<time.h>
#include<stdlib.h>

/*
long long int key[15][15][2] = { 0 };//第三维度分别代表黑子和白子，0为空白
long long int hashing_value[depth_of_hashing][3] = { {0,0,0} };
//第二维度的[0]是整个棋盘的哈希值，[1][2]是此哈希值对应的棋盘评分，应该定义在函数外面
long long int hashing_value_now = 0;//目前的哈希值，这个是要在minimax.c里面进行运算的
*/

void initialize_hashing_sheet(long long int key[][15][2])
{//本程序每场比赛只要在最开始执行一次就行，不能多次执行
	int zob_raw, zob_column, zob_status;
	long long int a, b, c;
	srand(time(NULL));
	
	for (zob_raw = 0; zob_raw < 15; zob_raw++)
	{
		for (zob_column = 0; zob_column < 15; zob_column++)
		{
			for (zob_status = 0; zob_status < 2; zob_status++)
			{
				a = rand();
				b = rand();
				c = rand();
				key[zob_raw][zob_column][zob_status] = a * 10000 + b * 1000 + c;
				//printf("%lld ", key[zob_raw][zob_column][zob_status]);
			}
			//printf("\n");
		}
		//printf("\n\n");
	}
}

//↓此函数已废弃
long int Zobrist_hashing(long long int hashing_value[][3], long long int key[][15][2], 
	int best_raw, int best_column,
	//在实际引用的时候注意raw和column两个参数，因为这个函数是要记录整个棋盘的最佳得分，而不是单个点的得分
	//取而代之，打算使用别的函数，例如minimax里面的评分来作为输入，因此换成了2个value参数
	//raw和column两个参数一定要是最佳的raw和column，而不是循环遍历的raw和column
	//long int my_value, long int opponent_value,
	bool write_new_coordinate,
	//这个布尔值是控制此函数的功能，如果是true的话，此函数才会记录新的哈希值及其评分，否则仅仅作为一个查询哈希值的函数
	int step_count, char board[][17][3], 
	bool my_turn, long long int hashing_value_now)
{
	int black = 0;
	int white = 1;
	int chess;
	int index = 0;
	if ((step_count % 2) == 0)
	{
		chess = black;
	}
	else
	{
		chess = white;
	}
	
	for (index = 0; index < depth_of_hashing; index++)
	{
	//搜索的逻辑是，先找有没有非0的哈希值，
	//如果有，再看它是否与外界现在的哈希值相等，否则在第一个为0的位置记录下来这个值
		if (hashing_value[index][0] != 0)//如果这个哈希值不为0
		{
			if (hashing_value[index][0] == hashing_value_now)//如果这个哈希值与外面的哈希值相等
			{
				//这两行是用来测试这张表是否有用，正常工作时要消除的
				exit(0);
				//在表中就找到了对应的哈希值，那就返回对应的评分
				if (my_turn)
				{//目前[1]记录的是我方的得分
					return hashing_value[index][1];
				}
				else//[2]记录的是对方的得分
				{
					return hashing_value[index][2];
				}
				
			}
			
		}
		else//如果这个哈希值为0，记录下来本次的哈希值和评分
		{
			if (write_new_coordinate)
			{
				hashing_value[index][0] = hashing_value[index][0] ^ key[best_raw][best_column][chess];
				hashing_value[index][1] = evaluation(board, step_count, my_turn, best_raw, best_column);
				hashing_value[index][2] = evaluation(board, step_count + 1, !my_turn, best_raw, best_column);
				return 0; //这个操作也只能执行一次，如果执行了，就必须跳出循环
			}
			
		}
		
		
		
	}
	
	//循环走完，剩下的情况是，哈希表满了，但是还是没对应的值
		
	return 0;
}

long int Searching_Hashing(unsigned long long hashing_value2[][3], unsigned long long ZobristTable[15][15][2],
	int step_count, unsigned long long hashValue, bool my_turn, long temp_score, bool write)
{
	int black = 0;
	int white = 1;
	int chess;
	int index = 0;
	if ((step_count % 2) == 0)
	{
		chess = black;
	}
	else
	{
		chess = white;
	}
	if (!write)//只读模式
	{

	
		for (index = 0; index < depth_of_hashing; index++)
		{
			//搜索的逻辑是，先找有没有非0的哈希值，
			//如果有，再看它是否与外界现在的哈希值相等，否则在第一个为0的位置记录下来这个值
			if (hashing_value2[index][0] != 0)//如果这个哈希值不为0
			{
				if (hashing_value2[index][0] == hashValue)//如果这个哈希值与外面的哈希值相等
				{
					//这两行是用来测试这张表是否有用，正常工作时要消除的
					printf("\nfind it\n");
					system("pause");
					//在表中就找到了对应的哈希值，那就返回对应的评分
					if (my_turn)
					{//目前[1]记录的是我方的得分
						return (long)hashing_value2[index][1];
					}
					else//[2]记录的是对方的得分
					{
						return (long)hashing_value2[index][2];
					}

				}

			}

		}

		//循环走完，剩下的情况是，哈希表满了，但是还是没对应的值

		return 0;

	}
	else//读写模式
	{

		for (index = 0; index < depth_of_hashing; index++)
		{
			if (hashing_value2[index][0] == 0)//如果这个哈希值为0
			{
				hashing_value2[index][0] = hashValue;
				if (my_turn)
				{
					hashing_value2[index][1] = (unsigned long long)temp_score;
				}
				else
				{
					hashing_value2[index][2] = (unsigned long long)temp_score;
				}


				return 0; //这个操作也只能执行一次，如果执行了，就必须跳出循环
			}

		}
		//循环走完，剩下的情况是，哈希表满了

		return 0;
	
		
	}


	

	
}



long int Searching_Hashing2(unsigned long long hashing_value2[][3], unsigned long long ZobristTable[15][15][2],
	int step_count, unsigned long long hashValue, bool my_turn, long temp_score, bool write)
{
	int black = 0;
	int white = 1;
	int chess;
	int index = 0;
	if ((step_count % 2) == 0)
	{
		chess = black;
	}
	else
	{
		chess = white;
	}
	if (!write)//只读模式
	{


		for (index = 0; index < depth_of_hashing; index++)
		{
			//搜索的逻辑是，先找有没有非0的哈希值，
			//如果有，再看它是否与外界现在的哈希值相等，否则在第一个为0的位置记录下来这个值
			if (hashing_value2[index][0] != 0)//如果这个哈希值不为0
			{
				if (hashing_value2[index][0] == hashValue)//如果这个哈希值与外面的哈希值相等
				{
					//这两行是用来测试这张表是否有用，正常工作时要消除的
					/*
					printf("\nfind it\n");
					system("pause");
					*/
					//在表中就找到了对应的哈希值，那就返回对应的评分
					if (my_turn)
					{//目前[1]记录的是我方的得分
						return (long)hashing_value2[index][1];
					}
					else//[2]记录的是对方的得分
					{
						return (long)hashing_value2[index][2];
					}

				}

			}

		}

		//循环走完，剩下的情况是，哈希表满了，但是还是没对应的值

		return 0;

	}
	else//读写模式
	{

		for (index = 0; index < depth_of_hashing; index++)
		{
			if (hashing_value2[index][0] == 0)//如果这个哈希值为0
			{
				hashing_value2[index][0] = hashValue;
				if (my_turn)
				{
					hashing_value2[index][1] = (unsigned long long)temp_score;
				}
				else
				{
					hashing_value2[index][2] = (unsigned long long)temp_score;
				}


				return 0; //这个操作也只能执行一次，如果执行了，就必须跳出循环
			}

		}
		//循环走完，剩下的情况是，哈希表满了
		printf("full!!\n");
		system("pause");
		return 0;


	}


}