#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include"head.h"
#include<time.h>

int board[15][15] = {0};
int coordinate[2] = { 0, 0 };
int w = 119;//白棋
int b = 98;//黑棋
int roaming = 0;//便于悔棋的，用来记录上一步的空格在哪个位置
unsigned long long ZobristTable[15][15][2];//梅森旋转的哈希键值表
unsigned long long hashValue = 0;//梅森旋转算法下，棋盘的哈希值
unsigned long long hashing_value2[depth_of_hashing][3] = { {0,0,0} };
//第二维度的[0]是整个棋盘的哈希值
//[1]与[2]是此哈希值对应的棋盘评分，应该定义在函数外面
//上面这个是新的棋盘哈希表，注意评分在输出的时候要进行类型转换

void main()
{
	//int step_count = 0; //游戏下了几个子的计数
	
	bool continue_playing = true; //确认游戏是否继续
	
	long int value = 0;//评分函数的打分
	long int my_value = 0;//我方得分
	long int opponent_value = 0;//对方得分
	int mode_choice = 0;


	initial_board();
	mode_choice = mode_choose(mode_choice);
	if (mode_choice == 1)
	{
		pvp(value);
	}
	else
	{
		pve(value);
	}

	//DrawBoard(value, mode_choice, step_count);
}

void myprintf(int array[], int length)
{
	int index = 0;
	for (index = 0; index < length; index++)
	{
		printf("%d,", array[index]);
	}
	printf("\n");
}

void initial_board()
{
	int raw = 0;
	int column = 0;
	for (raw = 0; raw < 15; raw++)
	{
		for (column = 0; column < 15; column++)
		{
			if (column == 0)
			{
				if (raw == 0)//(0,0)代表棋盘的左上角┏，用数字4代替
				{
					board[raw][column] = 4;
				}
				else if (raw == 14)//(14,0)代表棋盘的左下角┗，用数字5代替
				{
					board[raw][column] = 5;
				}
				else//其余的(x,0)代表棋盘的左边沿┠，用数字6代替
				{
					board[raw][column] = 6;
				}
			}
			else if (column == 14)
			{
				if (raw == 0)//(0,14)代表棋盘的右上角┓，用数字7代替
				{
					board[raw][column] = 7;
				}
				else if (raw == 14)//(14,14)代表棋盘的右下角┛，用数字8代替
				{
					board[raw][column] = 8;
				}
				else//其余的(x,14)代表棋盘的右边沿┨，用数字9代替
				{
					board[raw][column] = 9;
				}
			}
			else
			{
				if (raw == 0)//(0,x)都是棋盘的上边沿，用数字10代替
				{
					board[raw][column] = 10;
				}
				else if (raw == 14)//(14,x)都是棋盘的下边沿，用数字11代替
				{
					board[raw][column] = 11;
				}
				else//剩下的空的点都是普通的空位┼，用数字0代替
				{
					board[raw][column] = 0;
				}
			}
		}
	}
}

void DrawBoard(long int value, int mode_choice, int step_count)
{
	int raw, column = 0;

	char c_column;
	int i_raw;
	i_raw = 15 - coordinate[0];
	c_column = 'A' + coordinate[1];
	char *mode;
	if (mode_choice == 1)
	{
		mode = "PVP";
	}
	else
	{
		mode = "PVE";
	}
	system("cls");   //清屏
	printf("  ==Welcome to FiveInRow Game==\n");
	printf("          ==%s mode==\n\n", mode);
	printf("   A B C D E F G H I J K L M N O \n");

	for (raw = 0; raw < 15; raw++)
	{
		printf("%2d", 15 - raw);
		for (column = 0; column < 15; column++)
		{
			
			switch (board[raw][column])
			{
			case 0:
				printf("┼");
				break;
			case 4:
				printf("┏");
				break;
			case 5:
				printf("┗");
				break;
			case 6:
				printf("┠");
				break;
			case 7:
				printf("┓");
				break;
			case 8:
				printf("┛");
				break;
			case 9:
				printf("┨");
				break;
			case 10:
				printf("┯");
				break;
			case 11:
				printf("┷");
				break;
			case 119://119对应ASCII里面的小写字母w，即white
				printf("●");
				break;
			case 98://98对应b，即black
				printf("○");
				break;
			case 1://1对应的是刚下的那个倒三角形的黑色棋
				printf("△");
				break;
			case 2://2对应的是刚下的白色的棋
				printf("▲");
				break;
			default:
				break;
			}
			
		}
		printf("%2d\n", 15 - raw);
		
	}
	printf("   A B C D E F G H I J K L M N O \n");


	if (step_count != -1)
	{
		printf("最后落子(%d, %c), value = %ld\n", i_raw, c_column, value);
	}
}

int mode_choose(int mode_choice)
{
	
	int mode_choice_index;
	bool invalid_mode = true;

	printf("************************************************\n");
	printf("请选择模式\n");
	printf("1.pvp\t\t2.pve\n");
	printf("************************************************\n");
	while (invalid_mode)
	{

		mode_choice_index = scanf("%d", &mode_choice);
		while (getchar() != '\n')
			continue;
		if (mode_choice_index != 1)
		{
			printf("无效输入，请重试\n");

			continue;
		}
		if ((mode_choice != 1) && (mode_choice != 2))
		{
			printf("无效输入，请重试\n");

			continue;
		}

		invalid_mode = false;
	}


	return mode_choice;
}
