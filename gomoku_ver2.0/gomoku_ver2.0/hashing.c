#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include"head.h"
#include<time.h>
#include<stdlib.h>

extern int board[15][15];
extern int coordinate[2];
extern int roaming;
extern unsigned long long ZobristTable[15][15][2];//梅森旋转的哈希键值表
extern unsigned long long hashValue;//梅森旋转算法下，棋盘的哈希值
unsigned long long hashing_value2[depth_of_hashing][3];

long int Searching_Hashing(int step_count, bool my_turn, long temp_score, bool write)
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
						if (hashing_value2[index][1] != 0)
						{
							return (long)hashing_value2[index][1];
						}
					}
					else//[2]记录的是对方的得分
					{
						if (hashing_value2[index][2] != 0)
						{
							return (long)hashing_value2[index][2];
						}

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
			if (hashing_value2[index][0] == hashValue)//如果在记录的过程中发现已经存在已有的记录了
			{
				if (my_turn)
				{
					if ((hashing_value2[index][1] == 0) && (temp_score != 0))//当已有的记录中，我方得分为0（没有登记过我方得分），且将要登记的我方得分不为0，则登记我方得分
					{
						hashing_value2[index][1] = (unsigned long long)temp_score;
						return 0; //这个操作也只能执行一次，如果执行了，就必须跳出循环
					}
				}
				else
				{
					if ((hashing_value2[index][2] == 0) && (temp_score != 0))//当已有的记录中，对方得分为0（没有登记过对方得分），且将要登记的对方得分不为0，则登记对方得分
					{
						hashing_value2[index][2] = (unsigned long long)temp_score;
						return 0; //这个操作也只能执行一次，如果执行了，就必须跳出循环
					}
				}

			}


			else if (hashing_value2[index][0] == 0)//如果没有现成记录，则找一个哈希值为0的空位
			{

				if (my_turn)
				{
					if (temp_score != 0)//仅登记非0的得分
					{
						hashing_value2[index][0] = hashValue;
						hashing_value2[index][1] = (unsigned long long)temp_score;
					}
				}
				else
				{
					if (temp_score != 0)

					{
						hashing_value2[index][0] = hashValue;
						hashing_value2[index][2] = (unsigned long long)temp_score;
					}
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