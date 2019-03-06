#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#include"head.h"

//开局库

extern int board[15][15];
extern int coordinate[2];
extern int roaming;
extern int w;//白棋
extern int b;//黑棋
extern bool ai_first;


void open_base()
{
	int vector;
	//int empty_point[8][2] = { {6,6},{6,7},{6,8},{7,6},{7,8},{8,6},{8,7},{8,8} };
	srand(time(0));
	vector = rand() % 2 ? 1 : -1;//随机生成±1
	if (ai_first)
	{
		//下面是直指开局，使用花月
		//coordinate会存储上一步的落子位置，因此可以用它来判断是直指还是斜指
		if ((coordinate[0] == 7) &&
			((coordinate[1] == 6) || (coordinate[1] == 8)))//水平方向的直指，即白子落在了8I或者8G处
		{
			coordinate[0] += vector;
		}
		else if ((coordinate[1] == 7) &&
			((coordinate[0] == 6) || (coordinate[0] == 8)))
		{
			coordinate[1] += vector;
		}
		//下面是斜指开局，使用浦月
		else if ((coordinate[0] == 6 && coordinate[1] == 8)//9I浦月
			|| (coordinate[0] == 8 && coordinate[1] == 6))//7G浦月
		{
			coordinate[0] = 7 + vector;
			coordinate[1] = 7 + vector;//随机落在7I和9G处
		}
		else if ((coordinate[0] == 8 && coordinate[1] == 8)//7I浦月
			|| (coordinate[0] == 6 && coordinate[1] == 6))//9G浦月
		{
			coordinate[0] = 7 + vector;
			coordinate[1] = 7 - vector;//随机落在7G和9I处
		}
		else//不是斜指也不是直指
		{
			randomly_choose_a_point(7, 7);
		}
	}
	else
	{
		randomly_choose_a_point(coordinate[0], coordinate[1]);
	}
}

void randomly_choose_a_point(int raw, int column)
{
	int vector1;
	int vector2;
	do
	{
		vector1 = rand() % 3 - 1;
		vector2 = rand() % 3 - 1;
	} while (vector1 == 0 && vector2 == 0);

	coordinate[0] = raw + vector1;
	coordinate[1] = column + vector2;
}