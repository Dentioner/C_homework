#include<stdio.h>
#include<string.h>
#include<stdbool.h>

bool judgement(char board[][17][3], int step_count)
//两个参数，一个是棋盘，另一个是步数，用于判断是哪一方赢
//返回值先设成布尔型
{
	int i, j, k;
	bool continue_playing = true;
	bool not_full = false;//这个布尔值用于判断，棋盘是否铺满了
	char black[2] = "○";
	char white[2] = "●";
	char *chess;
	if (step_count % 2)//如果step数不能整除2的话，就是白子
	{
		chess = white;
	}
	else
	{
		chess = black;
	}
	
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 17; j++)
	//下面是水平方向有五颗子的判断
		{
			if ((strncmp(board[i][j], chess, 2) == 0)
				&& (strncmp(board[i][j + 1], chess, 2) == 0)
				&& (strncmp(board[i][j + 2], chess, 2) == 0)
				&& (strncmp(board[i][j + 3], chess, 2) == 0)
				&& (strncmp(board[i][j + 4], chess, 2) == 0))
			{
				continue_playing = false;
			}
	//下面是竖直方向有五个子的判断
			if ((strncmp(board[i][j], chess, 2) == 0)
				&& (strncmp(board[i + 1][j], chess, 2) == 0)
				&& (strncmp(board[i + 2][j], chess, 2) == 0)
				&& (strncmp(board[i + 3][j], chess, 2) == 0)
				&& (strncmp(board[i + 4][j], chess, 2) == 0))
			{
				continue_playing = false;
			}
	//下面是左上右下斜线的五子判断
			if ((strncmp(board[i][j], chess, 2) == 0)
				&& (strncmp(board[i+1][j + 1], chess, 2) == 0)
				&& (strncmp(board[i+2][j + 2], chess, 2) == 0)
				&& (strncmp(board[i+3][j + 3], chess, 2) == 0)
				&& (strncmp(board[i+4][j + 4], chess, 2) == 0))
			{
				continue_playing = false;
			}

	//下面是左下右上斜线的五子判断
			if ((strncmp(board[i][j], chess, 2) == 0)
				&& (strncmp(board[i+1][j -1], chess, 2) == 0)
				&& (strncmp(board[i+2][j -2], chess, 2) == 0)
				&& (strncmp(board[i+3][j - 3], chess, 2) == 0)
				&& (strncmp(board[i+4][j - 4], chess, 2) == 0))
			{
				continue_playing = false;
			}
		}
	}

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 17; j++)
		{
			if ((strncmp(board[i][j], "┏", 2) == 0)
				|| (strncmp(board[i][j], "┯", 2) == 0)
				|| (strncmp(board[i][j], "┓", 2) == 0)
				|| (strncmp(board[i][j], "┠", 2) == 0)
				|| (strncmp(board[i][j], "┼", 2) == 0)
				|| (strncmp(board[i][j], "┨", 2) == 0)
				|| (strncmp(board[i][j], "┗", 2) == 0)
				|| (strncmp(board[i][j], "┷", 2) == 0)
				|| (strncmp(board[i][j], "┛", 2) == 0))
			{
				not_full = true;
			}
		}
	}

	if (not_full == false)
		continue_playing = false;

	return continue_playing;

}