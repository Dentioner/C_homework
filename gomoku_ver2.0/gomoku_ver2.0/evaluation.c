#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"head.h"
//形态的定义见https://www.cnblogs.com/songdechiu/p/5768999.html
//https://baike.1688.com/doc/view-d3091513.html

extern int board[15][15];
extern int w;//白棋
extern int b;//黑棋
long int evaluation(int step_count, bool my_turn, int raw, int column)
	//step_count的作用是，确认这盘是黑子还是白子
	//my_turn 的作用是，确认这盘是我方还是敌方
	//raw和column是传递坐标，看看到底是哪个位置需要评估
{
	//先确认这把是白子下还是黑子下
	int chess;
	int opponent_chess;


	long int value = 0;
	if (step_count % 2)//如果step数不能整除2的话，就是白子
	{
		chess = w;
		opponent_chess = b;
	}
	else
	{
		chess = b;
		opponent_chess = w;
	}


	//检查连五
		//水平方向
			//●●●●?型
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess) 
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column - 4] == chess))
	{
		value += Consecutive_Five;
	}
	//●●●?●型
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column + 1] == chess))
	{
		value += Consecutive_Five;
	}

	//●●?●●型
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess))
	{
		value += Consecutive_Five;
	}

	//●?●●●型
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess))
	{
		value += Consecutive_Five;
	}

	//?●●●●型
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 4] == chess))
	{
		value += Consecutive_Five;
	}

	//竖直方向
		//●●●●?型
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw - 4][column] == chess))
	{
		value += Consecutive_Five;
	}

	//●●●?●型
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw + 1][column] == chess))
	{
		value += Consecutive_Five;
	}

	//●●?●●型
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess))
	{
		value += Consecutive_Five;
	}

	//●?●●●型

	if ((board[raw - 1][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess))
	{
		value += Consecutive_Five;
	}

	//?●●●●型
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 4][column] == chess))
	{
		value += Consecutive_Five;
	}

	//右上左下方向
		//●●●●?型

	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 4][column - 4] == chess))
	{
		value += Consecutive_Five;
	}

	//●●●?●型
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw - 1][column + 1] == chess))
	{
		value += Consecutive_Five;
	}

	//●●?●●型
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess))
	{
		value += Consecutive_Five;
	}

	//●?●●●型
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw + 1][column - 1] == chess))
	{
		value += Consecutive_Five;
	}

	//?●●●●型
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw - 4][column + 4] == chess))
	{
		value += Consecutive_Five;
	}

	//左上右下方向
		//●●●●?型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw - 4][column - 4] == chess))
	{
		value += Consecutive_Five;
	}

	//●●●?●型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw + 1][column + 1] == chess))
	{
		value += Consecutive_Five;
	}

	//●●?●●型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess))
	{
		value += Consecutive_Five;
	}

	//●?●●●型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess))
	{
		value += Consecutive_Five;
	}

	//?●●●●型
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 4][column + 4] == chess))
	{
		value += Consecutive_Five;
	}


	//检查活四
		//水平方向
			//_?●●●_型
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (column + 4 <= 14) && (column - 1 >= 0))
	{
		value += Open_Four;
	}

	//_●?●●_型
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column - 2] !=chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (column + 3 <= 14) && (column - 2 >= 0))
	{
		value += Open_Four;
	}

	//_●●?●_型
	if ((board[raw][column - 2] == chess)
		&& (board[raw][column - 1] == chess)
		&& (board[raw][column + 1] == chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (column - 3 >= 0) && (column + 2 <= 14))
	{
		value += Open_Four;
	}

	//_●●●?_型
	if ((board[raw][column - 3] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 1] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0) && (column + 1 <= 14))
	{
		value += Open_Four;
	}

	//竖直方向
		//_?●●●_型
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (raw + 4 <= 14) && (raw - 1 >= 0))
	{
		value += Open_Four;
	}

	//_●?●●_型

	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw - 1][column] == chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw - 2 >= 0) && (raw + 3 <= 14))
	{
		value += Open_Four;
	}

	//_●●?●_型
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw - 3 >= 0) && (raw + 2 <= 14))
	{
		value += Open_Four;
	}

	//_●●●?_型
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0) && (raw + 1 <= 14))
	{
		value += Open_Four;
	}

	//右上左下型
		//_?●●●_型
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (raw + 1 <= 14) && (column + 4 <= 14) && (column - 1 >= 0))
	{
		value += Open_Four;
	}

	//_●?●●_型
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (raw + 2 <= 14) && (raw - 3 >= 0) && (column - 2 >= 0) && (column + 3 <= 14))
	{
		value += Open_Four;
	}

	//_●●?●_型
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (raw - 2 >= 0) && (raw + 3 <= 14) && (column + 2 <= 14) && (column - 3 >= 0))
	{
		value += Open_Four;
	}

	//_●●●?_型
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (raw - 1 >= 0) && (raw + 4 <= 14) && (column + 1 <= 14) && (column - 4 >= 0))
	{
		value += Open_Four;
	}

	//左上右下型
		//_?●●●_型
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (raw + 4 <= 14) && (raw - 1 >= 0) && (column - 1 >= 0) && (column + 4 <= 14))
	{
		value += Open_Four;
	}

	//_●?●●_型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (raw - 2 >= 0) && (raw + 3 <= 14) && (column - 2 >= 0) && (column + 3 <= 14))
	{
		value += Open_Four;
	}

	//_●●?●_型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (raw - 3 >= 0) && (raw + 2 <= 14) && (column - 3 >= 0) && (column + 2 <= 14))
	{
		value += Open_Four;
	}

	//_●●●?_型
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (raw + 1 <= 14) && (column + 1 <= 14) && (column - 4 >= 0))
	{
		value += Open_Four;
	}

	//活三和冲四中的一种一起判断
	//分别是形如_●●●__ 和 _●●●_●的
	//各自倒过来又是一种：__●●●_和 ●_●●●_
	//先来上面那两种
		//水平方向
			//_?●●__或_?●●_●
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (column - 1 >= 0))
	{
		if ((board[raw][column + 4] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw][column + 4] != chess)
			&& (board[raw][column + 4] != opponent_chess)
			&& (column + 4 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●?●__或_●?●_●
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 1] == chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (column - 2 >= 0))
	{
		if ((board[raw][column + 3] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw][column + 3] != chess)
			&& (board[raw][column + 3] != opponent_chess)
			&& (column + 3 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●●?__或_●●?_●
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (column - 3 >= 0))
	{
		if ((board[raw][column + 2] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw][column + 2] != chess)
			&& (board[raw][column + 2] != opponent_chess)
			&& (column + 2 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●●●_?,这种情况只有冲四有
	if ((board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column - 4] == chess)
		&& (board[raw][column - 5] != chess)
		&& (board[raw][column - 5] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (column - 5 >= 0))
	{
		value += Gapped_Four;
	}

	//竖直方向
		//_?●●__或_?●●_●
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw - 1 >= 0))
	{
		if ((board[raw + 4][column] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 4][column] != chess)
			&& (board[raw + 4][column] != opponent_chess)
			&& (raw + 4 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●?●__或_●?●_●
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (raw - 2 >= 0))
	{
		if ((board[raw + 3][column] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 3][column] != chess)
			&& (board[raw + 3][column] != opponent_chess)
			&& (raw + 3 <= 14))
		{
			value += Open_Three;
		}
	}


	//_●●?__或_●●?_●
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw - 3 >= 0))
	{
		if ((board[raw + 2][column] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 2][column] != chess)
			&& (board[raw + 2][column] != opponent_chess)
			&& (raw + 2 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●●●_?,这种情况只有冲四有
	if ((board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw - 4][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 5][column] != chess)
		&& (board[raw - 5][column] != opponent_chess)
		&& (raw - 5 >= 0))
	{
		value += Gapped_Four;
	}

	//右上左下的情况
		//_?●●__或_?●●_●
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (raw + 1 <= 14) && (column - 1 >= 0))
	{
		if ((board[raw - 4][column + 4] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 4][column + 4] != chess)
			&& (board[raw - 4][column + 4] != opponent_chess)
			&& (raw - 4 >= 0) && (column + 4 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●?●__或_●?●_●
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column - 2 >= 0))
	{
		if ((board[raw - 3][column + 3] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 3][column + 3] != chess)
			&& (board[raw - 3][column + 3] != opponent_chess)
			&& (raw - 3 >= 0) && (column + 3 <= 14))
		{
			value += Open_Three;
		}
	}


	//_●●?__或_●●?_●
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column - 3 >= 0))
	{
		if ((board[raw - 2][column + 2] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 2][column + 2] != chess)
			&& (board[raw - 2][column + 2] != opponent_chess)
			&& (raw - 2 >= 0) && (column + 2 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●●●_?,这种情况只有冲四有
	if ((board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 4][column - 4] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 5][column - 5] != chess)
		&& (board[raw + 5][column - 5] != opponent_chess)
		&& (raw + 5 <= 14) && (column - 5 >= 0))
	{
		value += Gapped_Four;
	}

	//左上右下方向
		//_?●●__或_?●●_●
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (raw - 1 >= 0) && (column - 1 >= 0))
	{
		if ((board[raw + 4][column + 4] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 4][column + 4] != chess)
			&& (board[raw + 4][column + 4] != opponent_chess)
			&& (raw + 4 <= 14) && (column + 4 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●?●__或_●?●_●
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column - 2 >= 0))
	{
		if ((board[raw + 3][column + 3] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 3][column + 3] != chess)
			&& (board[raw + 3][column + 3] != opponent_chess)
			&& (raw + 3 <= 14) && (column + 3 <= 14))
		{
			value += Open_Three;
		}
	}


	//_●●?__或_●●?_●
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column - 3 >= 0))
	{
		if ((board[raw + 2][column + 2] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 2][column + 2] != chess)
			&& (board[raw + 2][column + 2] != opponent_chess)
			&& (raw + 2 <= 14) && (column + 2 <= 14))
		{
			value += Open_Three;
		}
	}

	//_●●●_?,这种情况只有冲四有
	if ((board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw - 4][column - 4] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 5][column - 5] != chess)
		&& (board[raw - 5][column - 5] != opponent_chess)
		&& (raw - 5 >= 0) && (column - 5 >= 0))
	{
		value += Gapped_Four;
	}

	//活三和冲四中的一种一起判断
	//分别是形如_●●●__ 和 _●●●_●的
	//各自倒过来又是一种：__●●●_和 ●_●●●_
	//现在是下面那两种  需要排除这种情况__●●●__，这种就和之前的重复了。
	//还需要排除__●●●_●这个情况
		//水平方向
			//__●●?_和 ●_●●?_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)//排除__●●●_●这个情况
		&& (column + 1 <= 14))
	{
		if ((board[raw][column - 4] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw][column - 4] != chess)
			&& (board[raw][column - 4] != opponent_chess)
			&& (column - 4 >= 0))
		{
			value += Open_Three;

			if ((board[raw][column + 2] != chess)
				&& (board[raw][column + 2] != opponent_chess)
				&& (column + 2 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//__●?●_和 ●_●?●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 1] == chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)//排除__●●●_●这个情况
		&& (column + 2 <= 14))
	{
		if ((board[raw][column - 3] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw][column - 3] != chess)
			&& (board[raw][column - 3] != opponent_chess)
			&& (column - 3 >= 0))
		{
			value += Open_Three;
			if ((board[raw][column + 3] != chess)
				&& (board[raw][column + 3] != opponent_chess)
				&& (column + 3 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//__?●●_和 ●_?●●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 4] != chess)//排除__●●●_●这个情况
		&& (column + 3 <= 14))
	{
		if ((board[raw][column - 2] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw][column - 2] != chess)
			&& (board[raw][column - 2] != opponent_chess)
			&& (column - 2 >= 0))
		{
			value += Open_Three;
			if ((board[raw][column + 4] != chess)
				&& (board[raw][column + 4] != opponent_chess)
				&& (column + 4 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//?_●●●_这种情况只有冲四有
	if ((board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 4] == chess)
		&& (board[raw][column + 5] != chess)
		&& (board[raw][column + 5] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 6] != chess)//排除__●●●_●这个情况
		&& (column + 5 <= 14))
	{
		value += Gapped_Four;
	}

	//竖直方向
		//__●●?_和 ●_●●?_
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)//排除__●●●_●这个情况
		&& (raw + 1 <= 14))
	{
		if ((board[raw - 4][column] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 4][column] != chess)
			&& (board[raw - 4][column] != opponent_chess)
			&& (raw - 4 >= 0))
		{
			value += Open_Three;
			if ((board[raw + 2][column] != chess)
				&& (board[raw + 2][column] != opponent_chess)
				&& (raw + 2 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//__●?●_和 ●_●?●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 1][column] == chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)//排除__●●●_●这个情况
		&& (raw + 2 <= 14))
	{
		if ((board[raw - 3][column] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 3][column] != chess)
			&& (board[raw - 3][column] != opponent_chess)
			&& (raw - 3 >= 0))
		{
			value += Open_Three;
			if ((board[raw + 3][column] != chess)
				&& (board[raw + 3][column] != opponent_chess)
				&& (raw + 3 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//__?●●_和 ●_?●●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)//排除__●●●_●这个情况
		&& (raw + 3 <= 14))
	{
		if ((board[raw - 2][column] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 2][column] != chess)
			&& (board[raw - 2][column] != opponent_chess)
			&& (raw - 2 >= 0))
		{
			value += Open_Three;
			if ((board[raw + 4][column] != chess)
				&& (board[raw + 4][column] != opponent_chess)
				&& (raw + 4 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//?_●●●_这种情况只有冲四有
	if ((board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 4][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 5][column] != chess)
		&& (board[raw + 5][column] != opponent_chess)
		&& (board[raw + 6][column] != chess)//排除__●●●_●这个情况
		&& (raw + 5 <= 14))
	{
		value += Gapped_Four;
	}



	//右上左下方向
		//__●●?_和 ●_●●?_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)//排除__●●●_●这个情况
		&& (raw - 1 >= 0) && (column + 1 <= 14))
	{
		if ((board[raw + 4][column - 4] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 4][column - 4] != chess)
			&& (board[raw + 4][column - 4] != opponent_chess)
			&& (raw + 4 <= 14) && (column - 4 >= 0))
		{
			value += Open_Three;
			if ((board[raw - 2][column + 2] != chess)
				&& (board[raw - 2][column + 2] != opponent_chess)
				&& (raw - 2 >= 0) && (column + 2 <= 14))
			{
				value -= Open_Three;
			}
		}
	}


	//__●?●_和 ●_●?●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)//排除__●●●_●这个情况
		&& (raw - 2 >= 0) && (column + 2 <= 14))
	{
		if ((board[raw + 3][column - 3] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 3][column - 3] != chess)
			&& (board[raw + 3][column - 3] != opponent_chess)
			&& (raw + 3 <= 14) && (column - 3 >= 0))
		{
			value += Open_Three;
			if ((board[raw - 3][column + 3] != chess)
				&& (board[raw - 3][column + 3] != opponent_chess)
				&& (raw - 3 >= 0) && (column + 3 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//__?●●_和 ●_?●●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)//排除__●●●_●这个情况
		&& (raw - 3 >= 0) && (column + 3 <= 14))
	{
		if ((board[raw + 2][column - 2] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw + 2][column - 2] != chess)
			&& (board[raw + 2][column - 2] != opponent_chess)
			&& (raw + 2 <= 14) && (column - 2 >= 0))
		{
			value += Open_Three;
			if ((board[raw - 4][column + 4] != chess)
				&& (board[raw - 4][column + 4] != opponent_chess)
				&& (raw - 4 >= 0) && (column + 4 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//?_●●●_这种情况只有冲四有
	if ((board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw - 4][column + 4] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 5][column + 5] != chess)
		&& (board[raw - 5][column + 5] != opponent_chess)
		&& (board[raw - 6][column + 6] != chess)//排除__●●●_●这个情况
		&& (raw - 5 >= 0) && (column + 5 <= 14))
	{
		value += Gapped_Four;
	}

	//左上右下方向
		//__●●?_和 ●_●●?_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)//排除__●●●_●这个情况
		&& (raw + 1 <= 14) && (column + 1 <= 14))
	{
		if ((board[raw - 4][column - 4] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 4][column - 4] != chess)
			&& (board[raw - 4][column - 4] != opponent_chess)
			&& (raw - 4 >= 0) && (column - 4 >= 0))
		{
			value += Open_Three;
			if ((board[raw + 2][column + 2] != chess)
				&& (board[raw + 2][column + 2] != opponent_chess)
				&& (raw + 2 <= 14) && (column + 2 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//__●?●_和 ●_●?●_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)//排除__●●●_●这个情况
		&& (raw + 2 <= 14) && (column + 2 <= 14))
	{
		if ((board[raw - 3][column - 3] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 3][column - 3] != chess)
			&& (board[raw - 3][column - 3] != opponent_chess)
			&& (raw - 3 >= 0)&(column - 3 >= 0))
		{
			value += Open_Three;
			if ((board[raw + 3][column + 3] != chess)
				&& (board[raw + 3][column + 3] != opponent_chess)
				&& (raw + 3 <= 14) && (column + 3 <= 14))
			{
				value -= Open_Three;
			}
		}
	}


	//__?●●_和 ●_?●●_
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)//排除__●●●_●这个情况
		&& (raw + 3 <= 14) && (column + 3 <= 14))
	{
		if ((board[raw - 2][column - 2] == chess))
		{
			value += Gapped_Four;
		}
		else if ((board[raw - 2][column - 2] != chess)
			&& (board[raw - 2][column - 2] != opponent_chess)
			&& (raw - 2 >= 0)&(column - 2 >= 0))
		{
			value += Open_Three;
			if ((board[raw + 4][column + 4] != chess)
				&& (board[raw + 4][column + 4] != opponent_chess)
				&& (raw + 4 <= 14) && (column + 4 <= 14))
			{
				value -= Open_Three;
			}
		}
	}

	//?_●●●_这种情况只有冲四有
	if ((board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 4][column + 4] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 5][column + 5] != chess)
		&& (board[raw + 5][column + 5] != opponent_chess)
		&& (board[raw + 6][column + 6] != chess)//排除__●●●_●这个情况
		&& (raw + 5 <= 14) && (column + 5 <= 14))
	{
		value += Gapped_Four;
	}



	//检查冲四●●_●●Gapped22
		//水平方向
			//?●_●●
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 4] == chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●?_●●
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_?●
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_●?
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column - 4] == chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//竖直方向
		//?●_●●
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 4][column] == chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●?_●●
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_?●
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_●?
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw - 4][column] == chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//右上左下方向
		//?●_●●
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw - 4][column + 4] == chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●?_●●
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_?●
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_●?
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 4][column - 4] == chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//左上右下方向
		//?●_●●
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 4][column + 4] == chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●?_●●
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_?●
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}

	//●●_●?
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw - 4][column - 4] == chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess))
	{
		value += Gapped_Two_Two;
	}


	//检查冲四Capped_Four ○●●●●_，还有一种反方向的在下面，这里先写一种方向
		// 水平方向
			// ○?●●●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& ((board[raw][column - 1] == opponent_chess) || (column - 1 < 0))
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14))
	{
		value += Capped_Four;
	}

	// ○●?●●_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& ((board[raw][column - 2] == opponent_chess) || (column - 2 < 0))
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (column + 3 <= 14))
	{
		value += Capped_Four;
	}
	// ○●●?●_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column + 1] == chess)
		&& ((board[raw][column - 3] == opponent_chess)|| (column - 3 < 0))
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (column + 2 <= 14))
	{
		value += Capped_Four;
	}
	// ○●●●?_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& ((board[raw][column - 4] == opponent_chess)|| (column - 4 < 0))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (column + 1 <= 14))
	{
		value += Capped_Four;
	}
	//竖直方向
		// ○?●●●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& ((board[raw - 1][column] == opponent_chess)|| (raw - 1 < 0))
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14))
	{
		value += Capped_Four;
	}

	// ○●?●●_

	if ((board[raw - 1][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& ((board[raw - 2][column] == opponent_chess)|| (raw - 2 < 0))
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw + 3 <= 14))
	{
		value += Capped_Four;
	}
	// ○●●?●_
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& ((board[raw - 3][column] == opponent_chess)|| (raw - 3 < 0))
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (raw + 2 <= 14))
	{
		value += Capped_Four;
	}
	// ○●●●?_
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& ((board[raw - 4][column] == opponent_chess)|| (raw - 4 < 0))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (raw + 1 <= 14))
	{
		value += Capped_Four;
	}

	//右上左下方向
		// ○?●●●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& ((board[raw + 1][column - 1] == opponent_chess)|| (raw + 1 > 14) || (column - 1 < 0))
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column + 4 <= 14))
	{
		value += Capped_Four;
	}

	// ○●?●●_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& ((board[raw + 2][column - 2] == opponent_chess)|| (raw + 2 > 14) || (column - 2 < 0))
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column + 3 <= 14))
	{
		value += Capped_Four;
	}

	// ○●●?●_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw - 1][column + 1] == chess)
		&& ((board[raw + 3][column - 3] == opponent_chess)|| (raw + 3 > 14) || (column - 3 < 0))
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column + 2 <= 14))
	{
		value += Capped_Four;
	}

	// ○●●●?_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& ((board[raw + 4][column - 4] == opponent_chess)|| (raw + 4 > 14) || (column - 4 < 0))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (raw - 1 >= 0) && (column + 1 <= 14))
	{
		value += Capped_Four;
	}

	//左上右下方向
		// ○?●●●_
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& ((board[raw - 1][column - 1] == opponent_chess)|| (raw - 1 < 0) || (column - 1 < 0))
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column + 4 <= 14))
	{
		value += Capped_Four;
	}

	// ○●?●●_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& ((board[raw - 2][column - 2] == opponent_chess)|| (raw - 2 < 0) || (column - 2 < 0))
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column + 3 <= 14))
	{
		value += Capped_Four;
	}

	// ○●●?●_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& ((board[raw - 3][column - 3] == opponent_chess)|| (raw - 3 < 0) || (column - 3 < 0))
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column + 2 <= 14))
	{
		value += Capped_Four;
	}

	// ○●●●?_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& ((board[raw - 4][column - 4] == opponent_chess)|| (raw - 4 < 0) || (column - 4 < 0))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (raw + 1 <= 14) && (column + 1 <= 14))
	{
		value += Capped_Four;
	}

	//冲四的另外一种方向_●●●●○
		// 水平方向
			// _●●●?○
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& ((board[raw][column + 1] == opponent_chess)|| (column + 1 > 14))
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0))
	{
		value += Capped_Four;
	}
	// _●●?●○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& ((board[raw][column + 2] == opponent_chess)|| (column + 2 > 14))
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (column - 3 >= 0))
	{
		value += Capped_Four;
	}

	// _●?●●○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column - 1] == chess)
		&& ((board[raw][column + 3] == opponent_chess)|| (column + 3 > 14))
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (column - 2 >= 0))
	{
		value += Capped_Four;
	}

	// _?●●●○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& ((board[raw][column + 4] == opponent_chess)|| (column + 4 > 14))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (column - 1 >= 0))
	{
		value += Capped_Four;
	}

	//竖直方向
		// _●●●?○
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& ((board[raw + 1][column] == opponent_chess)|| (raw + 1 > 14))
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0))
	{
		value += Capped_Four;
	}

	// _●●?●○
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& ((board[raw + 2][column] == opponent_chess)|| (raw + 2 > 14))
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw - 3 >= 0))
	{
		value += Capped_Four;
	}

	// _●?●●○
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw - 1][column] == chess)
		&& ((board[raw + 3][column] == opponent_chess)|| (raw + 3 > 14))
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (raw - 2 >= 0))
	{
		value += Capped_Four;
	}

	// _?●●●○
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& ((board[raw + 4][column] == opponent_chess)|| (raw + 4 > 14))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (raw - 1 >= 0))
	{
		value += Capped_Four;
	}

	//右上左下方向
		// _●●●?○
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& ((board[raw - 1][column + 1] == opponent_chess)|| (raw - 1 < 0) || (column + 1 > 14))
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column - 4 >= 0))
	{
		value += Capped_Four;
	}

	// _●●?●○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& ((board[raw - 2][column + 2] == opponent_chess)|| (raw - 2 < 0) || (column + 2 > 14))
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column - 3 >= 0))
	{
		value += Capped_Four;
	}

	// _●?●●○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& ((board[raw - 3][column + 3] == opponent_chess)|| (raw - 3 < 0) || (column + 3 > 14))
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column - 2 >= 0))
	{
		value += Capped_Four;
	}

	// _?●●●○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& ((board[raw - 4][column + 4] == opponent_chess)|| (raw - 4 < 0) || (column + 4 > 14))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (raw + 1 <= 14) && (column - 1 >= 0))
	{
		value += Capped_Four;
	}

	//左上右下方向
		// _●●●?○
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& ((board[raw + 1][column + 1] == opponent_chess)|| (raw + 1 > 14) || (column + 1 > 14))
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column - 4 >= 0))
	{
		value += Capped_Four;
	}

	// _●●?●○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& ((board[raw + 2][column + 2] == opponent_chess)|| (raw + 2 > 14) || (column + 2 > 14))
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column - 3 >= 0))
	{
		value += Capped_Four;
	}


	// _●?●●○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw - 1][column - 1] == chess)
		&& ((board[raw + 3][column + 3] == opponent_chess)|| (raw + 3 > 14) || (column + 3 > 14))
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column - 2 >= 0))
	{
		value += Capped_Four;
	}

	// _?●●●○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& ((board[raw + 4][column + 4] == opponent_chess)|| (raw + 4 > 14) || (column + 4 > 14))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (raw - 1 >= 0) && (column - 1 >= 0))
	{
		value += Capped_Four;
	}


	//检查跳活三Gapped_Three  _●_●●_与_●●_●_

		//水平方向
		//先检查_●_●●_
			//_?_●●_
	if ((board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14) && (column - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//_●_?●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (column - 3 >= 0) && (column + 2 <= 14))
	{
		value += Gapped_Three;
	}

	//_●_●?_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0) && (column + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//再检查_●●_●_
		//_●●_?_
	if ((board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0) && (column + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//_●?_●_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 2 >= 0))
	{
		value += Gapped_Three;
	}

	//_?●_●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14) && (column - 1 >= 0))
	{
		value += Gapped_Three;
	}


	//竖直方向
	//先检查_●_●●_
		//_?_●●_
	if ((board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14) && (raw - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//_●_?●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw - 3 >= 0) && (raw + 2 <= 14))
	{
		value += Gapped_Three;
	}

	//_●_●?_
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0) && (raw + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//再检查_●●_●_
		//_●●_?_
	if ((board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0) && (raw + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//_●?_●_
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw + 3 <= 14) && (raw - 2 >= 0))
	{
		value += Gapped_Three;
	}

	//_?●_●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14) && (raw - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//右上左下方向
	//先检查_●_●●_
		//_?_●●_
	if ((board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column + 4 <= 14) && (raw + 1 <= 14) && (column - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//_●_?●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column - 3 >= 0) && (raw - 2 >= 0) && (column + 2 <= 14))
	{
		value += Gapped_Three;
	}

	//_●_●?_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column - 4 >= 0) && (raw - 1 >= 0) && (column + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//再检查_●●_●_
		//_●●_?_
	if ((board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column - 4 >= 0) && (raw - 1 >= 0) && (column + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//_●?_●_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column + 3 <= 14) && (raw + 2 <= 14) && (column - 2 >= 0))
	{
		value += Gapped_Three;
	}

	//_?●_●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column + 4 <= 14) && (raw + 1 <= 14) && (column - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//左上右下方向
	//先检查_●_●●_
		//_?_●●_
	if ((board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column + 4 <= 14) && (raw - 1 >= 0) && (column - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//_●_?●_
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column - 3 >= 0) && (raw + 2 <= 14) && (column + 2 <= 14))
	{
		value += Gapped_Three;
	}

	//_●_●?_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column - 4 >= 0) && (raw + 1 <= 14) && (column + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//再检查_●●_●_
		//_●●_?_
	if ((board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column - 4 >= 0) && (raw + 1 <= 14) && (column + 1 <= 14))
	{
		value += Gapped_Three;
	}

	//_●?_●_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column + 3 <= 14) && (raw - 2 >= 0) && (column - 2 >= 0))
	{
		value += Gapped_Three;
	}

	//_?●_●_
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column + 4 <= 14) && (raw - 1 >= 0) && (column - 1 >= 0))
	{
		value += Gapped_Three;
	}

	//检查眠三：Capped_Three
	//1. ○●●●__与__●●●○型
		//水平方向
		//先是○●●●__
			//○?●●__

	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& ((board[raw][column - 1] == opponent_chess)|| (column - 1 < 0))
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?●__

	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 1] == chess)
		&& ((board[raw][column - 2] == opponent_chess)|| (column - 2 < 0))
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (column + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●?__

	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& ((board[raw][column - 3] == opponent_chess)|| (column - 3 < 0))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (column + 2 <= 14))
	{
		value += Capped_Three;
	}

	//再是__●●●○
		//__●●?○

	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& ((board[raw][column + 1] == opponent_chess)|| (column + 1 > 14))
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//__●?●○

	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 1] == chess)
		&& ((board[raw][column + 2] == opponent_chess)|| (column + 2 > 14))
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (column - 3 >= 0))
	{
		value += Capped_Three;
	}

	//__?●●○

	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& ((board[raw][column + 3] == opponent_chess)|| (column + 3 > 14))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (column - 2 >= 0))
	{
		value += Capped_Three;
	}

	//竖直方向
	//先是○●●●__
		//○?●●__

	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& ((board[raw - 1][column] == opponent_chess)|| (raw - 1 < 0))
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?●__

	if ((board[raw - 1][column] == chess)
		&& (board[raw + 1][column] == chess)
		&& ((board[raw - 2][column] == opponent_chess)|| (raw - 2 < 0))
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●?__

	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& ((board[raw - 3][column] == opponent_chess)|| (raw - 3 < 0))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (raw + 2 <= 14))
	{
		value += Capped_Three;
	}

	//再是__●●●○
		//__●●?○

	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& ((board[raw + 1][column] == opponent_chess)|| (raw + 1 > 14))
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0))
	{
		value += Capped_Three;
	}

	//__●?●○

	if ((board[raw + 1][column] == chess)
		&& (board[raw - 1][column] == chess)
		&& ((board[raw + 2][column] == opponent_chess)|| (raw + 2 > 14))
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw - 3 >= 0))
	{
		value += Capped_Three;
	}

	//__?●●○

	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& ((board[raw + 3][column] == opponent_chess)|| (raw + 3 > 14))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (raw - 2 >= 0))
	{
		value += Capped_Three;
	}

	//右上左下方向
	//先是○●●●__
		//○?●●__

	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& ((board[raw + 1][column - 1] == opponent_chess)|| (raw + 1 > 14) || (column - 1 < 0))
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?●__

	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 1][column + 1] == chess)
		&& ((board[raw + 2][column - 2] == opponent_chess)|| (raw + 2 > 14) || (column - 2 < 0))
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●?__

	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& ((board[raw + 3][column - 3] == opponent_chess)|| (raw + 3 > 14) || (column - 3 < 0))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column + 2 <= 14))
	{
		value += Capped_Three;
	}

	//再是__●●●○
		//__●●?○

	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& ((board[raw - 1][column + 1] == opponent_chess)|| (raw - 1 < 0) || (column + 1 > 14))
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//__●?●○

	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& ((board[raw - 2][column + 2] == opponent_chess)|| (raw - 2 < 0) || (column + 2 > 14))
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column - 3 >= 0))
	{
		value += Capped_Three;
	}

	//__?●●○

	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& ((board[raw - 3][column + 3] == opponent_chess)|| (raw - 3 < 0) || (column + 3 > 14))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column - 2 >= 0))
	{
		value += Capped_Three;
	}

	//左上右下方向
	//先是○●●●__
		//○?●●__

	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& ((board[raw - 1][column - 1] == opponent_chess)|| (raw - 1 < 0) || (column - 1 < 0))
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?●__

	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] == chess)
		&& ((board[raw - 2][column - 2] == opponent_chess)|| (raw - 2 < 0) || (column - 2 < 0))
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●?__

	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& ((board[raw - 3][column - 3] == opponent_chess)|| (raw - 3 < 0) || (column - 3 < 0))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column + 2 <= 14))
	{
		value += Capped_Three;
	}

	//再是__●●●○
		//__●●?○

	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& ((board[raw + 1][column + 1] == opponent_chess)|| (raw + 1 > 14) || (column + 1 > 14))
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//__●?●○

	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 1][column - 1] == chess)
		&& ((board[raw + 2][column + 2] == opponent_chess)|| (raw + 2 > 14) || (column + 2 > 14))
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column - 3 >= 0))
	{
		value += Capped_Three;
	}

	//__?●●○

	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& ((board[raw + 3][column + 3] == opponent_chess)|| (raw + 3 > 14) || (column + 3 > 14))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column - 2 >= 0))
	{
		value += Capped_Three;
	}

	//2.○●●_●_与_●_●●○
		//水平方向
		//先是○●●_●_
			//○?●_●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 3] == chess)
		&& ((board[raw][column - 1] == opponent_chess)|| (column - 1 < 0))
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?_●_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& ((board[raw][column - 2] == opponent_chess)|| (column - 2 < 0))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (column + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●_?_
	if ((board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& ((board[raw][column - 4] == opponent_chess)|| (column - 4 < 0))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (column + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●_●●○
		//_●_●?○
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 3] == chess)
		&& ((board[raw][column + 1] == opponent_chess)|| (column + 1 > 14))
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●_?●○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& ((board[raw][column + 2] == opponent_chess)|| (column + 2 > 14))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (column - 3 >= 0))
	{
		value += Capped_Three;
	}

	//_?_●●○
	if ((board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& ((board[raw][column + 4] == opponent_chess)|| (column + 4 > 14))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (column - 1 >= 0))
	{
		value += Capped_Three;
	}

	//竖直方向
	//先是○●●_●_
		//○?●_●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& ((board[raw - 1][column] == opponent_chess)|| (raw - 1 < 0))
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?_●_
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& ((board[raw - 2][column] == opponent_chess)|| (raw - 2 < 0))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●_?_
	if ((board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& ((board[raw - 4][column] == opponent_chess)|| (raw - 4 < 0))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (raw + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●_●●○
		//_●_●?○
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& ((board[raw + 1][column] == opponent_chess)|| (raw + 1 > 14))
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●_?●○
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& ((board[raw + 2][column] == opponent_chess)|| (raw + 2 > 14))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw - 3 >= 0))
	{
		value += Capped_Three;
	}

	//_?_●●○
	if ((board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& ((board[raw + 4][column] == opponent_chess)|| (raw + 4 > 14))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (raw - 1 >= 0))
	{
		value += Capped_Three;
	}

	//右上左下方向
	//先是○●●_●_
		//○?●_●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& ((board[raw + 1][column - 1] == opponent_chess)|| (raw + 1 > 14) || (column - 1 < 0))
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?_●_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& ((board[raw + 2][column - 2] == opponent_chess)|| (raw + 2 > 14) || (column - 2 < 0))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●_?_
	if ((board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& ((board[raw + 4][column - 4] == opponent_chess)|| (raw + 4 > 14) || (column - 4 < 0))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (raw - 1 >= 0) && (column + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●_●●○
		//_●_●?○
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& ((board[raw - 1][column + 1] == opponent_chess)|| (raw - 1 < 0) || (column + 1 > 14))
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●_?●○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& ((board[raw - 2][column + 2] == opponent_chess)|| (raw - 2 < 0) || (column + 2 > 14))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column - 3 >= 0))
	{
		value += Capped_Three;
	}

	//_?_●●○
	if ((board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& ((board[raw - 4][column + 4] == opponent_chess)|| (raw - 4 < 0) || (column + 4 > 14))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (raw + 1 <= 14) && (column - 1 >= 0))
	{
		value += Capped_Three;
	}

	//左上右下方向
	//先是○●●_●_
		//○?●_●_
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& ((board[raw - 1][column - 1] == opponent_chess)|| (raw - 1 < 0) || (column - 1 < 0))
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●?_●_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& ((board[raw - 2][column - 2] == opponent_chess)|| (raw - 2 < 0) || (column - 2 < 0))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (raw + 3 <= 14) && (column + 3 <= 14))
	{
		value += Capped_Three;
	}

	//○●●_?_
	if ((board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& ((board[raw - 4][column - 4] == opponent_chess)|| (raw - 4 < 0) || (column - 4 < 0))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (raw + 1 <= 14) && (column + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●_●●○
		//_●_●?○
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& ((board[raw + 1][column + 1] == opponent_chess)|| (raw + 1 > 14) || (column + 1 > 14))
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●_?●○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& ((board[raw + 2][column + 2] == opponent_chess)|| (raw + 2 > 14) || (column + 2 > 14))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (raw - 3 >= 0) && (column - 3 >= 0))
	{
		value += Capped_Three;
	}

	//_?_●●○
	if ((board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& ((board[raw + 4][column + 4] == opponent_chess)|| (raw + 4 > 14) || (column + 4 > 14))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (raw - 1 >= 0) && (column - 1 >= 0))
	{
		value += Capped_Three;
	}

	//3.○●_●●_与_●●_●○
		//水平方向
		//先是○●_●●_
			//○?_●●_
	if ((board[raw][column + 2] == chess)
		&& (board[raw][column + 3] == chess)
		&& ((board[raw][column - 1] == opponent_chess)|| (column - 1 < 0))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●_?●_
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& ((board[raw][column - 3] == opponent_chess)|| (column - 3 < 0))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (column + 2 <= 14))
	{
		value += Capped_Three;
	}

	//○●_●?_
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 3] == chess)
		&& ((board[raw][column - 4] == opponent_chess)|| (column - 4 < 0))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (column + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●●_●○
		//_●●_?○
	if ((board[raw][column - 2] == chess)
		&& (board[raw][column - 3] == chess)
		&& ((board[raw][column + 1] == opponent_chess)|| (column + 1 > 14))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●?_●○
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& ((board[raw][column + 3] == opponent_chess)|| (column + 3 > 14))
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (column - 2 > 1))
	{
		value += Capped_Three;
	}

	//_?●_●○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 3] == chess)
		&& ((board[raw][column + 4] == opponent_chess)|| (column + 4 > 14))
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (column - 1 > 1))
	{
		value += Capped_Three;
	}

	//竖直方向
	//先是○●_●●_
		//○?_●●_
	if ((board[raw + 2][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& ((board[raw - 1][column] == opponent_chess)|| (raw - 1 < 0))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●_?●_
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& ((board[raw - 3][column] == opponent_chess)|| (raw - 3 < 0))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (raw + 2 <= 14))
	{
		value += Capped_Three;
	}

	//○●_●?_
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& ((board[raw - 4][column] == opponent_chess)|| (raw - 4 < 0))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (raw + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●●_●○
		//_●●_?○
	if ((board[raw - 2][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& ((board[raw + 1][column] == opponent_chess)|| (raw + 1 > 14))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●?_●○
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& ((board[raw + 3][column] == opponent_chess)|| (raw + 3 > 14))
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (raw - 2 >= 0))
	{
		value += Capped_Three;
	}

	//_?●_●○
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& ((board[raw + 4][column] == opponent_chess)|| (raw + 4 > 14))
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (raw - 1 >= 0))
	{
		value += Capped_Three;
	}

	//右上左下方向
	//先是○●_●●_
		//○?_●●_
	if ((board[raw - 2][column + 2] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& ((board[raw + 1][column - 1] == opponent_chess)|| (raw + 1 > 14) || (column - 1 < 0))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●_?●_
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& ((board[raw + 3][column - 3] == opponent_chess)|| (raw + 3 > 14) || (column - 3 < 0))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column + 2 <= 14))
	{
		value += Capped_Three;
	}

	//○●_●?_
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& ((board[raw + 4][column - 4] == opponent_chess)|| (raw + 4 > 14) || (column - 4 < 0))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (raw - 1 >= 0) && (column + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●●_●○
		//_●●_?○
	if ((board[raw + 2][column - 2] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& ((board[raw - 1][column + 1] == opponent_chess)|| (raw - 1 < 0) || (column + 1 > 14))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●?_●○
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& ((board[raw - 3][column + 3] == opponent_chess)|| (raw - 3 < 0) || (column + 3 > 14))
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column - 2 >= 0))
	{
		value += Capped_Three;
	}

	//_?●_●○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& ((board[raw - 4][column + 4] == opponent_chess)|| (raw - 4 < 0) || (column + 4 > 14))
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (raw + 1 <= 14) && (column - 1 >= 0))
	{
		value += Capped_Three;
	}

	//左上右下方向
	//先是○●_●●_
		//○?_●●_
	if ((board[raw + 2][column + 2] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& ((board[raw - 1][column - 1] == opponent_chess)|| (raw - 1 < 0) || (column - 1 < 0))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (raw + 4 <= 14) && (column + 4 <= 14))
	{
		value += Capped_Three;
	}

	//○●_?●_
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& ((board[raw - 3][column - 3] == opponent_chess)|| (raw - 3 < 0) || (column - 3 < 0))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (raw + 2 <= 14) && (column + 2 <= 14))
	{
		value += Capped_Three;
	}

	//○●_●?_
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& ((board[raw - 4][column - 4] == opponent_chess)|| (raw - 4 < 0) || (column - 4 < 0))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (raw + 1 <= 14) && (column + 1 <= 14))
	{
		value += Capped_Three;
	}

	//再是_●●_●○
		//_●●_?○
	if ((board[raw - 2][column - 2] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& ((board[raw + 1][column + 1] == opponent_chess)|| (raw + 1 > 14) || (column + 1 > 14))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (raw - 4 >= 0) && (column - 4 >= 0))
	{
		value += Capped_Three;
	}

	//_●?_●○
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& ((board[raw + 3][column + 3] == opponent_chess)|| (raw + 3 > 14) || (column + 3 > 14))
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (raw - 2 >= 0) && (column - 2 >= 0))
	{
		value += Capped_Three;
	}

	//_?●_●○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& ((board[raw + 4][column + 4] == opponent_chess)|| (raw + 4 > 14) || (column + 4 > 14))
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (raw - 1 >= 0) && (column - 1 >= 0))
	{
		value += Capped_Three;
	}
	//下面这种眠三有可能不太对
	//因为不知道是这种●●__●，还是_●●__●_之类的
	//4. ●●__●和●__●●
		//水平方向
		//先是●●__●
			//?●__●
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 4] == chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●?__●
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 3] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●●__?
	if ((board[raw][column - 3] == chess)
		&& (board[raw][column - 4] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//再是●__●●
		//●__●?
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 4] == chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●__?●
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 3] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//?__●●
	if ((board[raw][column + 3] == chess)
		&& (board[raw][column + 4] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//竖直方向
	//先是●●__●
		//?●__●
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 4][column] == chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●?__●
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 3][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●●__?
	if ((board[raw - 3][column] == chess)
		&& (board[raw - 4][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//再是●__●●
		//●__●?
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 4][column] == chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●__?●
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 3][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//?__●●
	if ((board[raw + 3][column] == chess)
		&& (board[raw + 4][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//右上左下方向
	//先是●●__●
		//?●__●
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 4][column + 4] == chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●?__●
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 3][column + 3] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●●__?
	if ((board[raw + 3][column - 3] == chess)
		&& (board[raw + 4][column - 4] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//再是●__●●
		//●__●?
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 4][column - 4] == chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●__?●
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 3][column - 3] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//?__●●
	if ((board[raw - 3][column + 3] == chess)
		&& (board[raw - 4][column + 4] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//左上右下方向
	//先是●●__●
		//?●__●
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 4][column + 4] == chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●?__●
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 3][column + 3] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●●__?
	if ((board[raw - 3][column - 3] == chess)
		&& (board[raw - 4][column - 4] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//再是●__●●
		//●__●?
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 4][column - 4] == chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●__?●
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 3][column - 3] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//?__●●
	if ((board[raw + 3][column + 3] == chess)
		&& (board[raw + 4][column + 4] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//5. ●_●_●
	//注意这种情况○●_●_●○与_●_●_●_是不太一样的
	//暂时先不考虑这两个的区别
		//水平方向
			//?_●_●
	if ((board[raw][column + 2] == chess)
		&& (board[raw][column + 4] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess))
	{
		value += Capped_Three;
	}
	//●_?_●
	if ((board[raw][column + 2] == chess)
		&& (board[raw][column - 2] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●_●_?
	if ((board[raw][column - 2] == chess)
		&& (board[raw][column - 4] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//竖直方向
		//?_●_●
	if ((board[raw + 2][column] == chess)
		&& (board[raw + 4][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess))
	{
		value += Capped_Three;
	}
	//●_?_●
	if ((board[raw + 2][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●_●_?
	if ((board[raw - 2][column] == chess)
		&& (board[raw - 4][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//右上左下方向
		//?_●_●
	if ((board[raw - 2][column + 2] == chess)
		&& (board[raw - 4][column + 4] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess))
	{
		value += Capped_Three;
	}
	//●_?_●
	if ((board[raw - 2][column + 2] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●_●_?
	if ((board[raw + 2][column - 2] == chess)
		&& (board[raw + 4][column - 4] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//左上右下方向
		//?_●_●
	if ((board[raw + 2][column + 2] == chess)
		&& (board[raw + 4][column + 4] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess))
	{
		value += Capped_Three;
	}
	//●_?_●
	if ((board[raw + 2][column + 2] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//●_●_?
	if ((board[raw - 2][column - 2] == chess)
		&& (board[raw - 4][column - 4] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess))
	{
		value += Capped_Three;
	}

	//6. ○_●●●_○型
		//水平方向
			//○_?●●_○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] == chess)
		&& ((board[raw][column - 2] == opponent_chess)|| (column - 2 < 0))
		&& ((board[raw][column + 4] == opponent_chess)|| (column + 4 > 14))
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●?●_○
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 1] == chess)
		&& ((board[raw][column + 3] == opponent_chess)|| (column + 3 > 14))
		&& ((board[raw][column - 3] == opponent_chess)|| (column - 3 < 0))
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●●?_○
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] == chess)
		&& ((board[raw][column + 2] == opponent_chess)|| (column + 2 > 14))
		&& ((board[raw][column - 4] == opponent_chess)|| (column - 4 < 0))
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//竖直方向
		//○_?●●_○
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] == chess)
		&& ((board[raw - 2][column] == opponent_chess)|| (raw - 2 < 0))
		&& ((board[raw + 4][column] == opponent_chess)|| (raw + 4 > 14))
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●?●_○
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 1][column] == chess)
		&& ((board[raw + 3][column] == opponent_chess)|| (raw + 3 > 14))
		&& ((board[raw - 3][column] == opponent_chess)|| (raw - 3 < 0))
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●●?_○
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] == chess)
		&& ((board[raw + 2][column] == opponent_chess)|| (raw + 2 > 14))
		&& ((board[raw - 4][column] == opponent_chess)|| (raw - 4 < 0))
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess))
	{
		value += Capped_Three;
	}

	//右上左下方向
		//○_?●●_○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] == chess)
		&& ((board[raw + 2][column - 2] == opponent_chess)|| (raw + 2 > 14) || (column - 2 < 0))
		&& ((board[raw - 4][column + 4] == opponent_chess)|| (raw - 4 < 0) || (column + 4 > 14))
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●?●_○
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 1][column - 1] == chess)
		&& ((board[raw - 3][column + 3] == opponent_chess)|| (raw - 3 < 0) || (column + 3 > 14))
		&& ((board[raw + 3][column - 3] == opponent_chess)|| (raw + 3 > 14) || (column - 3 < 0))
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●●?_○
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] == chess)
		&& ((board[raw - 2][column + 2] == opponent_chess)|| (raw - 2 < 0) || (column + 2 > 14))
		&& ((board[raw + 4][column - 4] == opponent_chess)|| (raw + 4 > 14) || (column - 4 < 0))
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess))
	{
		value += Capped_Three;
	}

	//左上右下方向
		//○_?●●_○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] == chess)
		&& ((board[raw - 2][column - 2] == opponent_chess)|| (raw - 2 < 0) || (column - 2 < 0))
		&& ((board[raw + 4][column + 4] == opponent_chess)|| (raw + 4 > 14) || (column + 4 > 14))
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess, 2)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess, 2))
	{
		value += Capped_Three;
	}

	//○_●?●_○
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 1][column - 1] == chess)
		&& ((board[raw + 3][column + 3] == opponent_chess)|| (raw + 3 > 14) || (column + 3 > 14))
		&& ((board[raw - 3][column - 3] == opponent_chess)|| (raw - 3 < 0) || (column - 3 < 0))
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess, 2)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess))
	{
		value += Capped_Three;
	}

	//○_●●?_○
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] == chess)
		&& ((board[raw + 2][column + 2] == opponent_chess)|| (raw + 2 > 14) || (column + 2 > 14))
		&& ((board[raw - 4][column - 4] == opponent_chess)|| (raw - 4 < 0) || (column - 4 < 0))
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess, 2)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess, 2))
	{
		value += Capped_Three;
	}
	//连二 ___●●__ & __●●___
		//水平方向
		//先检查___●●__
			//___?●__
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 3 >= 0))
	{
		value += Open_two;
	}

	//___●?__
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (board[raw][column - 4] != chess)
		&& (board[raw][column - 4] != opponent_chess)
		&& (column + 2 <= 14) && (column - 4 >= 0))
	{
		value += Open_two;
	}
	//再检查__●●___
		//__●?___
	if ((board[raw][column - 1] == chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column - 3] != chess)
		&& (board[raw][column - 3] != opponent_chess)
		&& (board[raw][column + 1] != chess)
		&& (board[raw][column + 1] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 3 >= 0))
	{
		value += Open_two;
		if ((board[raw][column - 4] != chess)
			&& (board[raw][column - 4] != opponent_chess)
			&& (column - 4 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//__?●___
	if ((board[raw][column + 1] == chess)
		&& (board[raw][column - 1] != chess)
		&& (board[raw][column - 1] != opponent_chess)
		&& (board[raw][column - 2] != chess)
		&& (board[raw][column - 2] != opponent_chess)
		&& (board[raw][column + 2] != chess)
		&& (board[raw][column + 2] != opponent_chess)
		&& (board[raw][column + 3] != chess)
		&& (board[raw][column + 3] != opponent_chess)
		&& (board[raw][column + 4] != chess)
		&& (board[raw][column + 4] != opponent_chess)
		&& (column + 4 <= 14) && (column - 2 >= 0))
	{
		value += Open_two;

		if ((board[raw][column - 3] != chess)
			&& (board[raw][column - 3] != opponent_chess)
			&& (column - 3 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//竖直方向
	//先检查___●●__
		//___?●__
	if ((board[raw + 1][column] == chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (raw + 3 <= 14) && (raw - 3 >= 0))
	{
		value += Open_two;
	}

	//___●?__
	if ((board[raw - 1][column] == chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (board[raw - 4][column] != chess)
		&& (board[raw - 4][column] != opponent_chess)
		&& (raw + 2 <= 14) && (raw - 4 >= 0))
	{
		value += Open_two;
	}
	//再检查__●●___
		//__●?___
	if ((board[raw - 1][column] == chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw - 3][column] != chess)
		&& (board[raw - 3][column] != opponent_chess)
		&& (board[raw + 1][column] != chess)
		&& (board[raw + 1][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (raw + 3 <= 14) && (raw - 3 >= 0))
	{
		value += Open_two;
		if ((board[raw - 4][column] != chess)
			&& (board[raw - 4][column] != opponent_chess)
			&& (raw - 4 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//__?●___
	if ((board[raw + 1][column] == chess)
		&& (board[raw - 1][column] != chess)
		&& (board[raw - 1][column] != opponent_chess)
		&& (board[raw - 2][column] != chess)
		&& (board[raw - 2][column] != opponent_chess)
		&& (board[raw + 2][column] != chess)
		&& (board[raw + 2][column] != opponent_chess)
		&& (board[raw + 3][column] != chess)
		&& (board[raw + 3][column] != opponent_chess)
		&& (board[raw + 4][column] != chess)
		&& (board[raw + 4][column] != opponent_chess)
		&& (raw + 4 <= 14) && (raw - 2 >= 0))
	{
		value += Open_two;
		if ((board[raw - 3][column] != chess)
			&& (board[raw - 3][column] != opponent_chess)
			&& (raw - 3 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//右上左下方向
		//先检查___●●__
			//___?●__
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 3 >= 0) && (raw + 3 <= 14) && (raw - 3 >= 0))
	{
		value += Open_two;
	}

	//___●?__
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (board[raw + 4][column - 4] != chess)
		&& (board[raw + 4][column - 4] != opponent_chess)
		&& (column + 2 <= 14) && (column - 4 >= 0) && (raw + 4 <= 14) && (raw - 2 >= 0))
	{
		value += Open_two;
	}
	//再检查__●●___
		//__●?___
	if ((board[raw + 1][column - 1] == chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw + 3][column - 3] != chess)
		&& (board[raw + 3][column - 3] != opponent_chess)
		&& (board[raw - 1][column + 1] != chess)
		&& (board[raw - 1][column + 1] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 3 >= 0) && (raw + 3 <= 14) && (raw - 3 >= 0))
	{
		value += Open_two;
		if ((board[raw + 4][column - 4] != chess)
			&& (board[raw + 4][column - 4] != opponent_chess)
			&& (raw + 4 <= 14) && (column - 4 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//__?●___
	if ((board[raw - 1][column + 1] == chess)
		&& (board[raw + 1][column - 1] != chess)
		&& (board[raw + 1][column - 1] != opponent_chess)
		&& (board[raw + 2][column - 2] != chess)
		&& (board[raw + 2][column - 2] != opponent_chess)
		&& (board[raw - 2][column + 2] != chess)
		&& (board[raw - 2][column + 2] != opponent_chess)
		&& (board[raw - 3][column + 3] != chess)
		&& (board[raw - 3][column + 3] != opponent_chess)
		&& (board[raw - 4][column + 4] != chess)
		&& (board[raw - 4][column + 4] != opponent_chess)
		&& (column + 4 <= 14) && (column - 2 >= 0) && (raw + 2 <= 14) && (raw - 4 >= 0))
	{
		value += Open_two;
		if ((board[raw + 3][column - 3] != chess)
			&& (board[raw + 3][column - 3] != opponent_chess)
			&& (raw + 3 <= 14) && (column - 3 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//左上右下方向
	//先检查___●●__
		//___?●__
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 3 >= 0) && (raw + 3 <= 14) && (raw - 3 >= 0))
	{
		value += Open_two;
	}

	//___●?__
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (board[raw - 4][column - 4] != chess)
		&& (board[raw - 4][column - 4] != opponent_chess)
		&& (column + 2 <= 14) && (column - 4 >= 0) && (raw + 2 <= 14) && (raw - 4 >= 0))
	{
		value += Open_two;
	}

	//再检查__●●___
		//__●?___
	if ((board[raw - 1][column - 1] == chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw - 3][column - 3] != chess)
		&& (board[raw - 3][column - 3] != opponent_chess)
		&& (board[raw + 1][column + 1] != chess)
		&& (board[raw + 1][column + 1] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (column + 3 <= 14) && (column - 3 >= 0) && (raw + 3 <= 14) && (raw - 3 >= 0))
	{
		value += Open_two;
		if ((board[raw - 4][column - 4] != chess)
			&& (board[raw - 4][column - 4] != opponent_chess)
			&& (raw - 4 >= 0) && (column - 4 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//__?●___
	if ((board[raw + 1][column + 1] == chess)
		&& (board[raw - 1][column - 1] != chess)
		&& (board[raw - 1][column - 1] != opponent_chess)
		&& (board[raw - 2][column - 2] != chess)
		&& (board[raw - 2][column - 2] != opponent_chess)
		&& (board[raw + 2][column + 2] != chess)
		&& (board[raw + 2][column + 2] != opponent_chess)
		&& (board[raw + 3][column + 3] != chess)
		&& (board[raw + 3][column + 3] != opponent_chess)
		&& (board[raw + 4][column + 4] != chess)
		&& (board[raw + 4][column + 4] != opponent_chess)
		&& (column + 4 <= 14) && (column - 2 >= 0) && (raw + 4 <= 14) && (raw - 2 >= 0))
	{
		value += Open_two;
		if ((board[raw - 3][column - 3] != chess)
			&& (board[raw - 3][column - 3] != opponent_chess)
			&& (raw - 3 >= 0) && (column - 3 >= 0))
		{//排除___●●___
			value -= Open_two;
		}
	}
	//如果这个是在评估对方的分数，就输出为负
	//现在的问题是，这个函数是己方对方各用一次，还是一个函数里面将双方都考虑一次？
	//目前的处理是，将这个函数己方对方各用一次，用布尔型my_turn区分


	if (!my_turn)
	{
		value *= -1;
	}
	return value;

}





