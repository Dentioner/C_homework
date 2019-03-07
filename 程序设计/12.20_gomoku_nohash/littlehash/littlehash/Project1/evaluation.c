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
	//int all_vector[4][2] = { {0,1}, {1,0}, {-1,1}, {1,1} };
	int i;
	/*这个循环还有问题，暂时不要用，这个配合all_vector一起使用，使用的时候将四个方位的注释掉
	for (i = 0; i<4 && value > Consecutive_Five; i++)
	{
		value += line(state, all_vector[i], raw, column, step_count);
	}
	*/
	value += line(state, horizon, raw, column, step_count);//水平计分
	value += line(state, perpendicular, raw, column, step_count);//垂直计分
	value += line(state, up_right_down_left, raw, column, step_count);//右上左下计分
	value += line(state, up_left_down_right, raw, column, step_count);//左上右下计分
	
	//如果这个是在评估对方的分数，就输出为负
	//现在的问题是，这个函数是己方对方各用一次，还是一个函数里面将双方都考虑一次？
	//目前的处理是，将这个函数己方对方各用一次，用布尔型my_turn区分

	if (value > Consecutive_Five)//再次修正得分，以排除不同方位叠加导致的分数超过连五
		value = Consecutive_Five;
	if (!my_turn)
		value *= -1;
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
	//bool special_state_for_four = false;//这个状态是用于判断同一直线上的双四棋型的，双三没有这种情况
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
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 5 * dx][column + 5 * dy] == chess)//_?●●●_●有一端不能下，不是活四
				&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
				&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
			{
				if ((board[raw - 2 * dx][column - 2 * dy] == chess)//●_?●●●_●是死四，没分
					&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
					&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))
				
				{
					value -= Open_Four;
				}
				else//X_?●●●_●是冲四，X代表不是黑子
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
			else if ((board[raw - 2 * dx][column - 2 * dy] == chess)//●_?●●●_另外一端不能下，不是活四
				&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
				&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))
			{
				if ((board[raw + 5 * dx][column + 5 * dy] == chess)//●_?●●●_●是死四，没分
					&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
					&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
				{
					value -= Open_Four;
				}
				else
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
		}
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
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 4 * dx][column + 4 * dy] == chess)//_●?●●_●有一端不能下，不是活四
				&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
				&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
			{
				if ((board[raw - 3 * dx][column - 3 * dy] == chess)//●_●?●●_●是死四，没分
					&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
					&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))

				{
					value -= Open_Four;
				}
				else//X_●?●●_●是冲四，X代表不是黑子
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
			else if ((board[raw - 3 * dx][column - 3 * dy] == chess)//●_●?●●_另外一端不能下，不是活四
				&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
				&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
			{
				if ((board[raw + 4 * dx][column + 4 * dy] == chess)//●_●?●●_●是死四，没分
					&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
					&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
				{
					value -= Open_Four;
				}
				else
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
		}
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
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 3 * dx][column + 3 * dy] == chess)//_●●?●_●有一端不能下，不是活四
				&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
				&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
			{
				if ((board[raw - 4 * dx][column - 4 * dy] == chess)//●_●●?●_●是死四，没分
					&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
					&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))

				{
					value -= Open_Four;
				}
				else//X_●●?●_●是冲四，X代表不是黑子
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
			else if ((board[raw - 4 * dx][column - 4 * dy] == chess)//●_●●?●_另外一端不能下，不是活四
				&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
				&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
			{
				if ((board[raw + 3 * dx][column + 3 * dy] == chess)//●_●●?●_●是死四，没分
					&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
					&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
				{
					value -= Open_Four;
				}
				else
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
		}
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
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 2 * dx][column + 2 * dy] == chess)//_●●●?_●有一端不能下，不是活四
				&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
				&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
			{
				if ((board[raw - 5 * dx][column - 5 * dy] == chess)//●_●●●?_●是死四，没分
					&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
					&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))

				{
					value -= Open_Four;
				}
				else//X_●●●?_●是冲四，X代表不是黑子
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
			else if ((board[raw - 5 * dx][column - 5 * dy] == chess)//●_●●●?_另外一端不能下，不是活四
				&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
				&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))
			{
				if ((board[raw + 2 * dx][column + 2 * dy] == chess)//●_●●●?_●是死四，没分
					&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
					&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
				{
					value -= Open_Four;
				}
				else
				{
					value -= Open_Four;
					value += Capped_Four;
				}
			}
		}
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
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 5 * dx][column + 5 * dy] == chess)//?●●_●●
				&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
				&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
			{
				state[direct_now] = false;
				value += Capped_Three;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw - 1 * dx][column - 1 * dy] == chess)//●?●●_●
				&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
				&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
		}
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
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 4 * dx][column + 4 * dy] == chess)//●?●_●●
				&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
				&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
			{
				state[direct_now] = false;
				value += Capped_Three;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw - 2 * dx][column - 2 * dy] == chess)//●●?●_●
				&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
				&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
		}
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
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 3 * dx][column + 3 * dy] == chess)//●●?_●●
				&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
				&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
			{
				state[direct_now] = false;
				value += Capped_Three;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw - 3 * dx][column - 3 * dy] == chess)//●●?●_●
				&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
				&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
		}
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
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Four;
			value += Double_Chess;
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 5 * dx][column - 5 * dy] == chess)//●●●●_?
				&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
				&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw + 1 * dx][column + 1 * dy] == chess)//●●●_?●
				&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
				&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
		}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 5 * dx][column - 5 * dy] == chess)//●●_●●?
				&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
				&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))
			{
				state[direct_now] = false;
				value += Capped_Three;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw + 1 * dx][column + 1 * dy] == chess)//●_●●?●
				&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
				&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 4 * dx][column - 4 * dy] == chess)//●●_●?●
				&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
				&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
			{
				state[direct_now] = false;
				value += Capped_Three;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw + 2 * dx][column + 2 * dy] == chess)//●_●?●●
				&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
				&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 3 * dx][column - 3 * dy] == chess)//●●_?●●
				&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
				&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
			{
				state[direct_now] = false;
				value += Capped_Three;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw + 3 * dx][column + 3 * dy] == chess)//●_?●●●
				&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
				&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 5 * dx][column + 5 * dy] == chess)//?_●●●●
				&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
				&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
			else if ((board[raw - 1 * dx][column - 1 * dy] == chess)//●?_●●●
				&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
				&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Four;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 5 * dx][column + 5 * dy] == chess)//?●_●●●
				&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
				&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
			else if ((board[raw - 1 * dx][column - 1 * dy] == chess)//●?●_●●
				&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
				&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 4 * dx][column + 4 * dy] == chess)//●?_●●●
				&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
				&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
			else if ((board[raw - 2 * dx][column - 2 * dy] == chess)//●●?_●●
				&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
				&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 2 * dx][column + 2 * dy] == chess)//●●_?●●
				&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
				&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
			else if ((board[raw - 4 * dx][column - 4 * dy] == chess)//●●●_?●
				&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
				&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 1 * dx][column + 1 * dy] == chess)//●●_●?●
				&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
				&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
			else if ((board[raw - 5 * dx][column - 5 * dy] == chess)//●●●_●?
				&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
				&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Gapped_Two_Two;
			}
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 5 * dx][column + 5 * dy] == chess)//○?●●●_●
				&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
				&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}
			
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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 4 * dx][column + 4 * dy] == chess)//○●?●●_●
				&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
				&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 3 * dx][column + 3 * dy] == chess)//○●●?●_●
				&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
				&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw + 2 * dx][column + 2 * dy] == chess)//○●●●?_●
				&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
				&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 5 * dx][column - 5 * dy] == chess)//●_●●●?○
				&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
				&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 4 * dx][column - 4 * dy] == chess)//●_●●?●○
				&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
				&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 3 * dx][column - 3 * dy] == chess)//●_●?●●○
				&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
				&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

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
		}
		state[direct_now] = true;
		if (chess == b)//黑子涉及禁手需要特殊修正
		{
			if ((board[raw - 2 * dx][column - 2 * dy] == chess)//●_?●●●○
				&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
				&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))
			{
				state[direct_now] = false;
				if (state[other_direct1] || state[other_direct2] || state[other_direct3])//这种情况下是要扣除双四双三的分数
					value -= Double_Chess;
				else
					value -= Capped_Four;
			}

		}
	}


	//检查扁担四：
	//X●●_●●_●●X & X●●●_●_●●●X & X●_●●●_●X
	//注意打X的位点不能是自己的子，否则那几个夹在中间的空位会产生长连禁手而不能落子
	//注意搜索位点只在中间部分，边上的那几个子不要搜
	//先检查X●●_●●_●●X
	//X●●_?●_●●X
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& ((board[raw + 5 * dx][column + 5 * dy] != chess) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dx < 0) || (column + 5 * dx > 14))
		&& ((board[raw - 4 * dx][column - 4 * dy] != chess) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dx < 0) || (column - 4 * dx > 14))

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value -= Gapped_Two_Two;
		value += Double_Chess;
	}
	//X●●_●?_●●X
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& ((board[raw + 4 * dx][column + 4 * dy] != chess) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dx < 0) || (column + 4 * dx > 14))
		&& ((board[raw - 5 * dx][column - 5 * dy] != chess) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dx < 0) || (column - 5 * dx > 14))

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value -= Gapped_Two_Two;
		value += Double_Chess;
	}
	//检查X●●●_●_●●●X
	//X●●●_?_●●●X
	if ((board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& ((board[raw + 5 * dx][column + 5 * dy] != chess) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dx < 0) || (column + 5 * dx > 14))
		&& ((board[raw - 5 * dx][column - 5 * dy] != chess) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dx < 0) || (column - 5 * dx > 14))

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value -= Gapped_Four;
		value += Double_Chess;
	}

	//检查X●_●●●_●X
	//X●_?●●_●X
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 4 * dx][column + 4 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& ((board[raw + 5 * dx][column + 5 * dy] != chess) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dx < 0) || (column + 5 * dx > 14))
		&& ((board[raw - 3 * dx][column - 3 * dy] != chess) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dx < 0) || (column - 3 * dx > 14))

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		value -= Gapped_Four;
		value += Double_Chess;
	}

	//X●_●?●_●X
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw + 3 * dx][column + 3 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 3 * dx][column - 3 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& ((board[raw + 4 * dx][column + 4 * dy] != chess) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dx < 0) || (column + 4 * dx > 14))
		&& ((board[raw - 4 * dx][column - 4 * dy] != chess) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dx < 0) || (column - 4 * dx > 14))

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		value -= Gapped_Four;
		value += Double_Chess;
	}

	//X●_●●?_●X
	if ((board[raw - 1 * dx][column - 1 * dy] == chess)
		&& (board[raw - 2 * dx][column - 2 * dy] == chess)
		&& (board[raw - 4 * dx][column - 4 * dy] == chess)
		&& (board[raw + 2 * dx][column + 2 * dy] == chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != chess)
		&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != chess)
		&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
		&& ((board[raw + 3 * dx][column + 3 * dy] != chess) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dx < 0) || (column + 3 * dx > 14))
		&& ((board[raw - 5 * dx][column - 5 * dy] != chess) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dx < 0) || (column - 5 * dx > 14))

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		value -= Gapped_Four;
		value += Double_Chess;
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
		state[direct_now] = true;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			
		}
		//排除_?_●●_●_
		if ((board[raw + 5 * dx][column + 5 * dy] == chess)
			&& (board[raw + 6 * dx][column + 6 * dy] != chess)
			&& (board[raw + 6 * dx][column + 6 * dy] != opponent_chess)

			&& (raw + 6 * dx >= 0) && (raw + 6 * dx <= 14)
			&& (column + 6 * dy >= 0) && (column + 6 * dy <= 14))
		{
			value -= Gapped_Three;
			if (chess == w)//对于白子来说这个棋型是双跳活三
				value += Double_Gapped_Three;
			else//但是对于黑子来讲这个棋型有禁手限制，是双眠三，或叫做六腐
				value += 2 * Capped_Three;
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
		state[direct_now] = true;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
			
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
		state[direct_now] = true;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
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
		state[direct_now] = true;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;			
		}
		//排除_●_●●_?_
		if ((board[raw - 5 * dx][column - 5 * dy] == chess)
			&& (board[raw - 6 * dx][column - 6 * dy] != chess)
			&& (board[raw - 6 * dx][column - 6 * dy] != opponent_chess)

			&& (raw - 6 * dx >= 0) && (raw - 6 * dx <= 14)
			&& (column - 6 * dy >= 0) && (column - 6 * dy <= 14))
		{
			value -= Gapped_Three;
			if (chess == w)//对于白子来说这个棋型是双跳活三
				value += Double_Gapped_Three;
			else//但是对于黑子来讲这个棋型有禁手限制，是双眠三，或叫做六腐
				value += 2 * Capped_Three;
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
		state[direct_now] = true;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;
		}
		//排除_●_●?_●_
		if ((board[raw - 3 * dx][column - 3 * dy] == chess)
			&& (board[raw - 4 * dx][column - 4 * dy] != chess)
			&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)

			&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
			&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
		{
			value -= 2 * Gapped_Three;
			if (chess == w)//对于白子来说这个棋型是双跳活三
				value += Double_Gapped_Three;
			else//但是对于黑子来讲这个棋型有禁手限制，是双眠三，或叫做六腐
				value += 2 * Capped_Three;
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
		state[direct_now] = true;
		if (state[other_direct1] || state[other_direct2] || state[other_direct3])
		{
			value -= Gapped_Three;
			value += Double_Chess;		
		}
		//排除_●_?●_●_
		if ((board[raw - 2 * dx][column - 2 * dy] == chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)

			&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
			&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))
		{
			value -= 2 * Gapped_Three;
			if (chess == w)//对于白子来说这个棋型是双跳活三
				value += Double_Gapped_Three;
			else//但是对于黑子来讲这个棋型有禁手限制，是双眠三，或叫做六腐
				value += 2 * Capped_Three;
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
		//下面这两行保证最外面两侧不是自己的子，否则像●●_●_●●的棋型，就会被这里检测到
		&& ((board[raw - 1 * dx][column - 1 * dy] != chess)||(raw - 1 * dx > 14) || (raw - 1 * dx < 0) || (column - 1 * dy > 14) || (column - 1 * dy < 0))
		&& ((board[raw + 5 * dx][column + 5 * dy] != chess) || (raw + 5 * dx > 14) || (raw + 5 * dx < 0) || (column + 5 * dy > 14) || (column + 5 * dy < 0))
		


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
		
		&& ((board[raw - 3 * dx][column - 3 * dy] != chess) || (raw - 3 * dx > 14) || (raw - 3 * dx < 0) || (column - 3 * dy > 14) || (column - 3 * dy < 0))
		&& ((board[raw + 3 * dx][column + 3 * dy] != chess) || (raw + 3 * dx > 14) || (raw + 3 * dx < 0) || (column + 3 * dy > 14) || (column + 3 * dy < 0))


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

		&& ((board[raw - 5 * dx][column - 5 * dy] != chess) || (raw - 5 * dx > 14) || (raw - 5 * dx < 0) || (column - 5 * dy > 14) || (column - 5 * dy < 0))
		&& ((board[raw + 1 * dx][column + 1 * dy] != chess) || (raw + 1 * dx > 14) || (raw + 1 * dx < 0) || (column + 1 * dy > 14) || (column + 1 * dy < 0))

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
	if ((board[raw + 1 * dx][column + 1 * dy] == chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != chess)
		&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != chess)
		&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != chess)
		&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != chess)
		&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != chess)
		&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)

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


	//以下是收录的特殊棋型，没法用上述规律计分，是由于禁手导致的计分漏洞，需要单独修正
	if (chess == b)//只有黑子才需要修正，白子跳过下面所有步骤
	{
		//1. X●●●_●●●X型，中间的空位是禁手，实际上这里是两个眠三
		//X●●●_?●●X
		if ((board[raw + 1 * dx][column + 1 * dy] == chess)
			&& (board[raw + 2 * dx][column + 2 * dy] == chess)
			&& (board[raw - 2 * dx][column - 2 * dy] == chess)
			&& (board[raw - 3 * dx][column - 3 * dy] == chess)
			&& (board[raw - 4 * dx][column - 4 * dy] == chess)
			&& (board[raw - 1 * dx][column - 1 * dy] != chess)
			&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
			&& ((board[raw + 3 * dx][column + 3 * dy] != chess) || (raw + 3 * dx > 14) || (raw + 3 * dx < 0) || (column + 3 * dy > 14) || (column + 3 * dy < 0))
			&& ((board[raw - 5 * dx][column - 5 * dy] != chess) || (raw - 5 * dx > 14) || (raw - 5 * dx < 0) || (column - 5 * dy > 14) || (column - 5 * dy < 0))
			&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
			&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
			&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
			&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
		{
			value = value - 2 * Gapped_Four - Gapped_Two_Two;
			if ((board[raw + 3 * dx][column + 3 * dy] != chess)
				&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
				&& (board[raw + 4 * dx][column + 4 * dy] != chess)
				&& (board[raw + 4 * dx][column + 4 * dy] != opponent_chess)
				&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
				&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))//●●●_?●●__时，还会多计入一个活三的分数，
			{
				value -= Open_Three;
			}
			value += 2 * Capped_Three;
		}

		//X●●?_●●●X
		if ((board[raw - 1 * dx][column - 1 * dy] == chess)
			&& (board[raw - 2 * dx][column - 2 * dy] == chess)
			&& (board[raw + 2 * dx][column + 2 * dy] == chess)
			&& (board[raw + 3 * dx][column + 3 * dy] == chess)
			&& (board[raw + 4 * dx][column + 4 * dy] == chess)
			&& (board[raw + 1 * dx][column + 1 * dy] != chess)
			&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
			&& ((board[raw - 3 * dx][column - 3 * dy] != chess) || (raw - 3 * dx > 14) || (raw - 3 * dx < 0) || (column - 3 * dy > 14) || (column - 3 * dy < 0))
			&& ((board[raw + 5 * dx][column + 5 * dy] != chess) || (raw + 5 * dx > 14) || (raw + 5 * dx < 0) || (column + 5 * dy > 14) || (column + 5 * dy < 0))
			&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
			&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14)
			&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
			&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))
		{
			value = value - 2 * Gapped_Four - Gapped_Two_Two;
			if ((board[raw - 3 * dx][column - 3 * dy] != chess)
				&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
				&& (board[raw - 4 * dx][column - 4 * dy] != chess)
				&& (board[raw - 4 * dx][column - 4 * dy] != opponent_chess)
				&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
				&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))//__●●?_●●●时，还会多计入一个活三的分数，
			{
				value -= Open_Three;
			}
			value += 2 * Capped_Three;
		}
		//X●●●_●?●X
		if ((board[raw + 1 * dx][column + 1 * dy] == chess)
			&& (board[raw - 1 * dx][column - 1 * dy] == chess)
			&& (board[raw - 3 * dx][column - 3 * dy] == chess)
			&& (board[raw - 4 * dx][column - 4 * dy] == chess)
			&& (board[raw - 5 * dx][column - 5 * dy] == chess)
			&& (board[raw - 2 * dx][column - 2 * dy] != chess)
			&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
			&& ((board[raw + 2 * dx][column + 2 * dy] != chess) || (raw + 2 * dx > 14) || (raw + 2 * dx < 0) || (column + 2 * dy > 14) || (column + 2 * dy < 0))
			&& ((board[raw - 6 * dx][column - 6 * dy] != chess) || (raw - 6 * dx > 14) || (raw - 6 * dx < 0) || (column - 6 * dy > 14) || (column - 6 * dy < 0))
			&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
			&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14)
			&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
			&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
		{
			value = value - Gapped_Four - Gapped_Two_Two;
			if ((board[raw + 2 * dx][column + 2 * dy] != chess)
				&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
				&& (board[raw + 3 * dx][column + 3 * dy] != chess)
				&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
				&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
				&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))//●●●_●?●__时，还会多计入一个活三的分数，
			{
				value -= Open_Three;
			}
			value += 2 * Capped_Three;
		}
		//X●?●_●●●X
		if ((board[raw - 1 * dx][column - 1 * dy] == chess)
			&& (board[raw + 1 * dx][column + 1 * dy] == chess)
			&& (board[raw + 3 * dx][column + 3 * dy] == chess)
			&& (board[raw + 4 * dx][column + 4 * dy] == chess)
			&& (board[raw + 5 * dx][column + 5 * dy] == chess)
			&& (board[raw + 2 * dx][column + 2 * dy] != chess)
			&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
			&& ((board[raw - 2 * dx][column - 2 * dy] != chess) || (raw - 2 * dx > 14) || (raw - 2 * dx < 0) || (column - 2 * dy > 14) || (column - 2 * dy < 0))
			&& ((board[raw + 6 * dx][column + 6 * dy] != chess) || (raw + 6 * dx > 14) || (raw + 6 * dx < 0) || (column + 6 * dy > 14) || (column + 6 * dy < 0))
			&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
			&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14)
			&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
			&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14))
		{
			value = value - Gapped_Four - Gapped_Two_Two;
			if ((board[raw - 2 * dx][column - 2 * dy] != chess)
				&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
				&& (board[raw - 3 * dx][column - 3 * dy] != chess)
				&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
				&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
				&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14))//__●?●_●●●时，还会多计入一个活三的分数，
			{
				value -= Open_Three;
			}
			value += 2 * Capped_Three;
		}
		//X●●●_●●?X
		if ((board[raw - 1 * dx][column - 1 * dy] == chess)
			&& (board[raw - 2 * dx][column - 2 * dy] == chess)
			&& (board[raw - 4 * dx][column - 4 * dy] == chess)
			&& (board[raw - 5 * dx][column - 5 * dy] == chess)
			&& (board[raw - 6 * dx][column - 6 * dy] == chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != chess)
			&& (board[raw - 3 * dx][column - 3 * dy] != opponent_chess)
			&& ((board[raw + 1 * dx][column + 1 * dy] != chess) || (raw + 1 * dx > 14) || (raw + 1 * dx < 0) || (column + 1 * dy > 14) || (column + 1 * dy < 0))
			&& ((board[raw - 7 * dx][column - 7 * dy] != chess) || (raw - 7 * dx > 14) || (raw - 7 * dx < 0) || (column - 7 * dy > 14) || (column - 7 * dy < 0))
			&& (raw - 6 * dx >= 0) && (raw - 6 * dx <= 14)
			&& (column - 6 * dy >= 0) && (column - 6 * dy <= 14))
		{
			value -= Gapped_Four;
			if ((board[raw + 1 * dx][column + 1 * dy] != chess)
				&& (board[raw + 1 * dx][column + 1 * dy] != opponent_chess)
				&& (board[raw + 2 * dx][column + 2 * dy] != chess)
				&& (board[raw + 2 * dx][column + 2 * dy] != opponent_chess)
				&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
				&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))//●●●_●●?__时，还会多计入一个活三的分数，
			{
				value -= Open_Three;
			}
			value += 2 * Capped_Three;
		}
		//X?●●_●●●X
		if ((board[raw + 1 * dx][column + 1 * dy] == chess)
			&& (board[raw + 2 * dx][column + 2 * dy] == chess)
			&& (board[raw + 4 * dx][column + 4 * dy] == chess)
			&& (board[raw + 5 * dx][column + 5 * dy] == chess)
			&& (board[raw + 6 * dx][column + 6 * dy] == chess)
			&& (board[raw + 3 * dx][column + 3 * dy] != chess)
			&& (board[raw + 3 * dx][column + 3 * dy] != opponent_chess)
			&& ((board[raw - 1 * dx][column - 1 * dy] != chess) || (raw - 1 * dx > 14) || (raw - 1 * dx < 0) || (column - 1 * dy > 14) || (column - 1 * dy < 0))
			&& ((board[raw + 7 * dx][column + 7 * dy] != chess) || (raw + 7 * dx > 14) || (raw + 7 * dx < 0) || (column + 7 * dy > 14) || (column + 7 * dy < 0))
			&& (raw + 6 * dx >= 0) && (raw + 6 * dx <= 14)
			&& (column + 6 * dy >= 0) && (column + 6 * dy <= 14))
		{
			value -= Gapped_Four;
			if ((board[raw - 1 * dx][column - 1 * dy] != chess)
				&& (board[raw - 1 * dx][column - 1 * dy] != opponent_chess)
				&& (board[raw - 2 * dx][column - 2 * dy] != chess)
				&& (board[raw - 2 * dx][column - 2 * dy] != opponent_chess)
				&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
				&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14))//__?●●_●●●时，还会多计入一个活三的分数，
			{
				value -= Open_Three;
			}
			value += 2 * Capped_Three;
		}
	}

	return value;
}