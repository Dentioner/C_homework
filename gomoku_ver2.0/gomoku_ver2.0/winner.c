#include<stdio.h>
#include<string.h>
#include<stdbool.h>
extern int board[15][15];
extern int w;//白棋
extern int b;//黑棋
bool judgement(int step_count)
//步数用于判断是哪一方赢
//返回值先设成布尔型
{
	int i, j, k;
	bool continue_playing = true;
	bool not_full = false;//这个布尔值用于判断，棋盘是否铺满了
	char *chess;
	if (step_count % 2)//如果step数不能整除2的话，就是白子
	{
		chess = w;
	}
	else
	{
		chess = b;
	}

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
			//下面是水平方向有五颗子的判断
		{
			if ((board[i][j] == chess)
				&& (board[i][j + 1] == chess)
				&& (board[i][j + 2] == chess)
				&& (board[i][j + 3] == chess)
				&& (board[i][j + 4] == chess))
			{
				continue_playing = false;
			}
			//下面是竖直方向有五个子的判断
			if ((board[i][j] == chess)
				&& (board[i + 1][j] == chess)
				&& (board[i + 2][j] == chess)
				&& (board[i + 3][j] == chess)
				&& (board[i + 4][j] == chess))
			{
				continue_playing = false;
			}
			//下面是左上右下斜线的五子判断
			if ((board[i][j] == chess)
				&& (board[i + 1][j + 1] == chess)
				&& (board[i + 2][j + 2] == chess)
				&& (board[i + 3][j + 3] == chess)
				&& (board[i + 4][j + 4] == chess))
			{
				continue_playing = false;
			}

			//下面是左下右上斜线的五子判断
			if ((board[i][j] == chess)
				&& (board[i + 1][j - 1] == chess)
				&& (board[i + 2][j - 2] == chess)
				&& (board[i + 3][j - 3] == chess)
				&& (board[i + 4][j - 4] == chess))
			{
				continue_playing = false;
			}
		}
	}

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			if ((board[i][j] == 4)
				|| (board[i][j] == 10)
				|| (board[i][j] == 7)
				|| (board[i][j] == 6)
				|| (board[i][j] == 0)
				|| (board[i][j] == 9)
				|| (board[i][j] == 5)
				|| (board[i][j] == 11)
				|| (board[i][j] == 8))
			{
				not_full = true;
			}
		}
	}

	if (not_full == false)
		continue_playing = false;

	return continue_playing;

}