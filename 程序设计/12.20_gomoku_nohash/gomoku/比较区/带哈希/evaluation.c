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
	long int value = 0;
	//int state = 0;//状态计数器，用于查找双活三双冲四之类的
	bool state[4] = { false, false, false, false };//状态记录，分为4个方向，分别是水平，竖直，右上左下，左上右下
	int horizon[2] = { 0,1 };//水平方向单位矢量
	int perpendicular[2] = { 1,0 };//垂直方向单位矢量
	int up_right_down_left[2] = { -1,1 };//右上左下方向单位矢量
	int up_left_down_right[2] = { 1,1 };//左上右下方向单位矢量

	value += line(state, horizon, raw, column, step_count);//水平计分
	value += line(state, perpendicular, raw, column, step_count);//垂直计分
	value += line(state, up_right_down_left, raw, column, step_count);//右上左下计分
	value += line(state, up_left_down_right, raw, column, step_count);//左上右下计分
	//如果这个是在评估对方的分数，就输出为负
	//现在的问题是，这个函数是己方对方各用一次，还是一个函数里面将双方都考虑一次？
	//目前的处理是，将这个函数己方对方各用一次，用布尔型my_turn区分


	if (!my_turn)
	{
		value *= -1;
	}
	return value;

}

long int line(bool state[], int vector[], int raw, int column, int step_count)//单一方向打分
{
	//vector是代表方向的单位矢量，分为{0,1}{1,0},{-1,1},{1,1}四种
	//分别代表水平方向，竖直方向，右上左下方向，左上右下方向
	int chess;
	int opponent_chess;
	long int value = 0;
	int dx = vector[0];
	int dy = vector[1];
	int direct_now, other_direct1, other_direct2, other_direct3;//用于帮助确定state的
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

	if (dx == 0 && dy == 1)//{0,1}
	{

		direct_now = 0;
		other_direct1 = 1;
		other_direct2 = 2;
		other_direct3 = 3;

	}
	else if (dx == 1 && dy == 0)//{1,0}
	{
		direct_now = 1;
		other_direct1 = 0;
		other_direct2 = 2;
		other_direct3 = 3;
	}
	else if (dx == -1 && dy == 1)//{-1,1}
	{
		direct_now = 2;
		other_direct1 = 0;
		other_direct2 = 1;
		other_direct3 = 3;
	}
	else if (dx == 1 && dy == 1)//{1,1}
	{
		direct_now = 3;
		other_direct1 = 0;
		other_direct2 = 1;
		other_direct3 = 2;
	}
	else
	{
		printf("error\n");
		system("pause");
	}
	//检查连五
		//以水平方向为例
			//●●●●?型
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Consecutive_Five;
	}
	//●●●?●型
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Consecutive_Five;
	}

	//●●?●●型
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Consecutive_Five;
	}

	//●?●●●型
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Consecutive_Five;
	}

	//?●●●●型
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Consecutive_Five;
	};


	//检查活四
		//水平方向
			//_?●●●_型
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Open_Four;
	}

	//_●?●●_型
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Open_Four;
	}

	//_●●?●_型
	if ((board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Open_Four;
	}

	//_●●●?_型
	if ((board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Open_Four;
	}

	//一种冲四
	//形如 ●●●_●的
	//倒过来又是一种：和 ●_●●●
	//先来上面那种
		//水平方向
			//?●●_●
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Gapped_Four;
		state[direct_now] = true;
	}

	//●?●_●
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Gapped_Four;
		state[direct_now] = true;
	}

	//●●?_●
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Gapped_Four;
		state[direct_now] = true;
	}

	//●●●_?,这种情况只有冲四有
	if ((board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Gapped_Four;
		state[direct_now] = true;
	}


	//冲四
	//●●●_●
	//倒过来又是一种：●_●●●
	//现在是下面那种

		//水平方向
			//和 ●_●●?
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Gapped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//●_●?●
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{

		value += Gapped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}


	//●_?●●
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Gapped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//?_●●●
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Gapped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//活三
	//分别是形如_●●●__
	//倒过来又是一种：__●●●_
	//先来上面那两种
		//水平方向
			//_?●●__
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)


		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Open_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Open_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//_●?●__
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Open_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Open_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//_●●?__
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Open_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Open_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//活三
	//分别是形如_●●●__ 
	//倒过来又是一种：__●●●_
	//现在是下面那种  需要排除这种情况__●●●__，这种就和之前的重复了。
	//还需要排除__●●●_●这个情况，这个就相当于在冲四里面再计入一个活三。这个是可能需要排除的，也可能不需要，看具体表现如何
		//水平方向
			//__●●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)//排除__●●●_●这个情况

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Open_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Open_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}

		if ((board[raw + 2 * dx][column + 2 * dy] != chess)
			&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

			&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
			&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
		{
			value -= Open_Three;
			if (state[other_direct1] || state[other_direct2] || state[other_direct3])
			{
				value += Open_Three;
				value -= Double_Chess;
				state[direct_now] = true;
			}

		}

	}

	//__●?●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)//排除__●●●_●这个情况

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Open_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Open_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
		if ((board[raw + 3 * dx][column + 3 * dy] != chess)
			&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

			&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
			&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
		{
			value -= Open_Three;
			if (state[other_direct1] || state[other_direct2] || state[other_direct3])
			{
				value += Open_Three;
				value -= Double_Chess;
				state[direct_now] = true;
			}

		}
	}

	//__?●●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)//排除__●●●_●这个情况

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Open_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Open_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
		if ((board[raw + 4 * dx][column + 4 * dy] != chess)
			&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

			&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
			&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
		{
			value -= Open_Three;
			if (state[other_direct1] || state[other_direct2] || state[other_direct3])
			{
				value += Open_Three;
				value -= Double_Chess;
				state[direct_now] = true;
			}

		}

	}

	//检查冲四●●_●●Gapped22
		//水平方向
			//?●_●●
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Gapped_Two_Two;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Two_Two;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//●?_●●
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Gapped_Two_Two;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Two_Two;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//●●_?●
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Gapped_Two_Two;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Two_Two;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//●●_●?
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Gapped_Two_Two;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Two_Two;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}


	//检查冲四Capped_Four ○●●●●_，还有一种反方向的在下面，这里先写一种方向
		// 水平方向
			// ○?●●●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& ((board[raw - 1 * dx][column - 1 * dy] == opponent_chess) || (raw - 1 * dx < 0) || (raw - 1 * dx > 14) || (column - 1 * dy < 0) || (column - 1 * dy > 14))
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	// ○●?●●_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& ((board[raw - 2 * dx][column - 2 * dy] == opponent_chess) || (raw - 2 * dx < 0) || (raw - 2 * dx > 14) || (column - 2 * dy < 0) || (column - 2 * dy > 14))
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}
	// ○●●?●_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& ((board[raw - 3 * dx][column - 3 * dy] == opponent_chess) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dy < 0) || (column - 3 * dy > 14))
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}
	// ○●●●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& ((board[raw - 4 * dx][column - 4 * dy] == opponent_chess) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dy < 0) || (column - 4 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}


	//冲四的另外一种方向_●●●●○
		// 水平方向
			// _●●●?○
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& ((board[raw + 1 * dx][column + 1 * dy] == opponent_chess) || (raw + 1 * dx < 0) || (raw + 1 * dx > 14) || (column + 1 * dy < 0) || (column + 1 * dy > 14))
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}
	// _●●?●○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& ((board[raw + 2 * dx][column + 2 * dy] == opponent_chess) || (raw + 2 * dx < 0) || (raw + 2 * dx > 14) || (column + 2 * dy < 0) || (column + 2 * dy > 14))
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	// _●?●●○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& ((board[raw + 3 * dx][column + 3 * dy] == opponent_chess) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dy < 0) || (column + 3 * dy > 14))
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	// _?●●●○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& ((board[raw + 4 * dx][column + 4 * dy] == opponent_chess) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dy < 0) || (column + 4 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Four;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Capped_Four;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//检查跳活三Gapped_Three  _●_●●_与_●●_●_
		//要排除_●_●●_●_这种情况
		//水平方向
		//先检查_●_●●_
			//_?_●●_
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Gapped_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//_●_?●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Gapped_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//_●_●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Gapped_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
	}

	//再检查_●●_●_
		//_●●_?_
	if ((board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Gapped_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
		//排除_●_●●_?_
		if ((board[raw - 5 * dx][column - 5 * dy] == chess)
			&& (board[raw - 6 * dx][column - 6 * dy] != chess)
			&& (board[raw - 6 * dx][column - 6 * dy] != opponent_chess)

			&& (raw - 6 * dx >= 0) && (raw - 6 * dx <= 14)
			&& (column - 6 * dy >= 0) && (column - 6 * dy <= 14))
		{
			value -= 2 * Gapped_Three;
			value += Double_Gapped_Three;
			if (state[other_direct1] || state[other_direct2] || state[other_direct3])
			{
				value += Gapped_Three;
				value -= Double_Chess;
				state[direct_now] = true;
			}
			else
			{
				state[direct_now] = true;
			}
		}
	}

	//_●?_●_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Gapped_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
		//排除_●_●?_●_
		if ((board[raw - 3 * dx][column - 3 * dy] == chess)
			&& (board[raw - 4 * dx][column - 4 * dy] != chess)
			&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

			&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
			&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
		{
			value -= 2 * Gapped_Three;
			value += Double_Gapped_Three;
			if (state[other_direct1] || state[other_direct2] || state[other_direct3])
			{
				value += Gapped_Three;
				value -= Double_Chess;
				state[direct_now] = true;
			}
			else
			{
				state[direct_now] = true;
			}
		}

	}

	//_?●_●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Gapped_Three;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			state[direct_now] = true;
		}
		else
		{
			state[direct_now] = true;
		}
		//排除_●_?●_●_
		if ((board[raw - 2 * dx][column - 2 * dy] == chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

			&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
			&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
		{
			value -= 2 * Gapped_Three;
			value += Double_Gapped_Three;
			if (state[other_direct1] || state[other_direct2] || state[other_direct3])
			{
				value += Gapped_Three;
				value -= Double_Chess;
				state[direct_now] = true;
			}
			else
			{
				state[direct_now] = true;
			}
		}
	}


	//检查眠三：Capped_Three
	//1. ○●●●__与__●●●○型
		//水平方向
		//先是○●●●__
			//○?●●__

	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& ((board[raw - 1 * dx][column - 1 * dy] == opponent_chess) || (raw - 1 * dx < 0) || (raw - 1 * dx > 14) || (column - 1 * dy < 0) || (column - 1 * dy > 14))
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○●?●__

	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] == chess)
		&& ((board[raw - 2 * dx][column - 2 * dy] == opponent_chess) || (raw - 2 * dx < 0) || (raw - 2 * dx > 14) || (column - 2 * dy < 0) || (column - 2 * dy > 14))
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○●●?__

	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& ((board[raw - 3 * dx][column - 3 * dy] == opponent_chess) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dy < 0) || (column - 3 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Three;
	}

	//再是__●●●○
		//__●●?○

	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& ((board[raw + 1 * dx][column + 1 * dy] == opponent_chess) || (raw + 1 * dx < 0) || (raw + 1 * dx > 14) || (column + 1 * dy < 0) || (column + 1 * dy > 14))
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//__●?●○

	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& ((board[raw + 2 * dx][column + 2 * dy] == opponent_chess) || (raw + 2 * dx < 0) || (raw + 2 * dx > 14) || (column + 2 * dy < 0) || (column + 2 * dy > 14))
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Three;
	}

	//__?●●○

	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& ((board[raw + 3 * dx][column + 3 * dy] == opponent_chess) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dy < 0) || (column + 3 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Three;
	}

	//2.○●●_●_与_●_●●○
		//水平方向
		//先是○●●_●_
			//○?●_●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& ((board[raw - 1 * dx][column - 1 * dy] == opponent_chess) || (raw - 1 * dx < 0) || (raw - 1 * dx > 14) || (column - 1 * dy < 0) || (column - 1 * dy > 14))
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○●?_●_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& ((board[raw - 2 * dx][column - 2 * dy] == opponent_chess) || (raw - 2 * dx < 0) || (raw - 2 * dx > 14) || (column - 2 * dy < 0) || (column - 2 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○●●_?_
	if ((board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& ((board[raw - 4 * dx][column - 4 * dy] == opponent_chess) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dy < 0) || (column - 4 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Three;
	}

	//再是_●_●●○
		//_●_●?○
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& ((board[raw + 1 * dx][column + 1 * dy] == opponent_chess) || (raw + 1 * dx < 0) || (raw + 1 * dx > 14) || (column + 1 * dy < 0) || (column + 1 * dy > 14))
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//_●_?●○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& ((board[raw + 2 * dx][column + 2 * dy] == opponent_chess) || (raw + 2 * dx < 0) || (raw + 2 * dx > 14) || (column + 2 * dy < 0) || (column + 2 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Three;
	}

	//_?_●●○
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& ((board[raw + 4 * dx][column + 4 * dy] == opponent_chess) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dy < 0) || (column + 4 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Three;
	}

	//3.○●_●●_与_●●_●○
		//水平方向
		//先是○●_●●_
			//○?_●●_
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& ((board[raw - 1 * dx][column - 1 * dy] == opponent_chess) || (raw - 1 * dx < 0) || (raw - 1 * dx > 14) || (column - 1 * dy < 0) || (column - 1 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○●_?●_
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& ((board[raw - 3 * dx][column - 3 * dy] == opponent_chess) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dy < 0) || (column - 3 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○●_●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& ((board[raw - 4 * dx][column - 4 * dy] == opponent_chess) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dy < 0) || (column - 4 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Three;
	}

	//再是_●●_●○
		//_●●_?○
	if ((board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& ((board[raw + 1 * dx][column + 1 * dy] == opponent_chess) || (raw + 1 * dx < 0) || (raw + 1 * dx > 14) || (column + 1 * dy < 0) || (column + 1 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//_●?_●○
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& ((board[raw + 3 * dx][column + 3 * dy] == opponent_chess) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dy < 0) || (column + 3 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Three;
	}

	//_?●_●○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& ((board[raw + 4 * dx][column + 4 * dy] == opponent_chess) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dy < 0) || (column + 4 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Three;
	}

	//下面这种眠三有可能不太对
	//因为不知道是这种●●__●，还是_●●__●_之类的
	//4. ●●__●和●__●●
		//水平方向
		//先是●●__●
			//?●__●
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//●?__●
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Three;
	}

	//●●__?
	if ((board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//再是●__●●
		//●__●?
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//●__?●
	if ((board[raw + 1 * dx][column + 1 * dx] == chess)
		&& (board[raw - 3 * dx][column - 3 * dx] == chess)
		&& (board[raw - 1 * dx][column - 1 * dx] != chess)
		&& (board[raw - 1 * dx][column - 1 * dx] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dx] != chess)
		&& (board[raw - 2 * dx][column - 2 * dx] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Three;
	}

	//?__●●
	if ((board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//5. ●_●_●
	//注意这种情况○●_●_●○与_●_●_●_是不太一样的
	//暂时先不考虑这两个的区别
		//水平方向
			//?_●_●
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Capped_Three;
	}
	//●_?_●
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Three;
	}

	//●_●_?
	if ((board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		value += Capped_Three;
	}

	//6. ○_●●●_○型
		//水平方向
			//○_?●●_○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& ((board[raw - 2 * dx][column - 2 * dy] == opponent_chess) || (raw - 2 * dx < 0) || (raw - 2 * dx > 14) || (column - 2 * dy < 0) || (column - 2 * dy > 14))
		&& ((board[raw + 4 * dx][column + 4 * dy] == opponent_chess) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dy < 0) || (column + 4 * dy > 14))
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess, 2)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess, 2)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○_●?●_○
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& ((board[raw + 3 * dx][column + 3 * dy] == opponent_chess) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dy < 0) || (column + 3 * dy > 14))
		&& ((board[raw - 3 * dx][column - 3 * dy] == opponent_chess) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dy < 0) || (column - 3 * dy > 14))
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess, 2)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Capped_Three;
	}

	//○_●●?_○
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& ((board[raw + 2 * dx][column + 2 * dy] == opponent_chess) || (raw + 2 * dx < 0) || (raw + 2 * dx > 14) || (column + 2 * dy < 0) || (column + 2 * dy > 14))
		&& ((board[raw - 4 * dx][column - 4 * dy] == opponent_chess) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dy < 0) || (column - 4 * dy > 14))
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess, 2)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess, 2)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		value += Capped_Three;
	}

	//连二 ___●●__ & __●●___
		//水平方向
		//先检查___●●__
			//___?●__
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Open_two;
	}

	//___●?__
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != chess)
		&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value += Open_two;
	}

	//再检查__●●___
		//__●?___
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value += Open_two;
		if ((board[raw - 4 * dx][column - 4 * dy] != chess)
			&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

			&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
			&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
		{//排除___●●___
			value -= Open_two;
		}
	}

	//__?●___
	if ((board[raw + 1 * dx][column + 1 * dx] == chess)
		&& (board[raw - 1 * dx][column - 1 * dx] != chess)
		&& (board[raw - 1 * dx][column - 1 * dx] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dx] != chess)
		&& (board[raw - 2 * dx][column - 2 * dx] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dx] != chess)
		&& (board[raw + 2 * dx][column + 2 * dx] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dx] != chess)
		&& (board[raw + 3 * dx][column + 3 * dx] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dx] != chess)
		&& (board[raw + 4 * dx][column + 4 * dx] != opponent_chess)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value += Open_two;
		if ((board[raw - 3 * dx][column - 3 * dy] != chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
			&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
			&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
		{//排除___●●___
			value -= Open_two;
		}
	}

	return value;
}