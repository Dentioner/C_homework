#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include"head.h"
#include<time.h>

extern int board[15][15];
extern int coordinate[2];
extern int roaming;


void pvp(long int value)
{
	int step_count = 0; //游戏下了几个子的计数
	bool continue_playing = true; //确认游戏是否继续
	bool my_turn = true; //这个东西是确认这一步是哪一方下子了
	bool find_forbidden_step = false;

		//下面这个是测试哈希表的
		/*
		for (int raw01 = 0; raw01 < 15; raw01++)
		{
			for (int raw02 = 0; raw02 < 15; raw02++)
			{

				printf("%ull, %ull\n", ZobristTable[raw01][raw02][0], ZobristTable[raw01][raw02][1]);

			}
		}
		system("pause");
		*/
	DrawBoard(value, 1, -1);
	while (continue_playing)
	{
		get_coordinate(step_count);
		roaming = board[coordinate[0]][coordinate[1]];
		chess_play_ver2(step_count);

		//测试用
		if (coordinate[0] == 6 && coordinate[1] == 9)
		{
			printf("\n");
		}
		
		
		if (step_count % 2 == 0)//黑子才执行判断禁手
		{
			find_forbidden_step = detect_forbidden_step(coordinate[0], coordinate[1]);
			if (find_forbidden_step)
			{
				printf("禁手\n");
				system("pause");
			}
		}
		value = 0;
		if (value == 0)
		{
			value = evaluation(step_count, my_turn, coordinate[0], coordinate[1]);
			
		}

		//my_value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
		//opponent_value = evaluation(board, step_count + 1, !my_turn, coordinate[0], coordinate[1]);
		//value = my_value + opponent_value;
		//上面这个分开打分的可能有问题
		DrawBoard(value, 1, step_count);
		return_to_normal_chess(step_count);



		printf("是否想要悔棋？按y悔棋，按别的任意键正常继续游戏.\n");
		char c_getback = ' ';
		c_getback = getchar();
		while (getchar() != '\n')
			continue;
		int i_getback = c_getback;
		if (i_getback == 89 || i_getback == 121)
		{
			board[coordinate[0]][coordinate[1]] = roaming;
			DrawBoard(value, 1, step_count);
		

			continue;
		}
		continue_playing = judgement(step_count);
		my_turn = !my_turn;
		step_count++;
	}
	DrawBoard(value, 1, step_count);
	if (step_count % 2)
	{
		printf("黑子获胜");
	}
	else
	{
		printf("白子获胜");
	}

	return;
}



void chess_play_ver2(int step_count)
{
	
	int black = 1;// "△"
	int white = 2;//"▲"
	int raw = coordinate[0];
	int column = coordinate[1];
	if (step_count % 2 == 0)
	{
		//strncpy(board[raw][column], black, 2);
		board[raw][column] = black;
	}
	else
	{
		//strncpy(board[raw][column], white, 2);
		board[raw][column] = white;
	}

}

void return_to_normal_chess(int step_count)
	//此函数是用来将△类型的棋子恢复成○类型的棋子
{
	
	int white = 119;//白棋
	int black = 98;//黑棋
	int raw = coordinate[0];
	int column = coordinate[1];
	if (step_count % 2 == 0)
	{
		board[raw][column] = black;
	}
	else
	{
		board[raw][column] = white;
	}
}


