#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"head.h"

extern int board[15][15];
extern int w;//白棋
extern int b;//黑棋
//判断禁手的函数

//本函数自身没有区分是白子执行还是黑子执行，也就是说如果一个禁手空位里面准备放白棋，函数还是会检测出禁手
//最好是在外面调用此函数的时候看看是白子走还是黑子走
bool detect_forbidden_step(int raw, int column)
{
	int horizon[2] = { 0,1 };//水平方向单位矢量
	int perpendicular[2] = { 1,0 };//垂直方向单位矢量
	int up_right_down_left[2] = { -1,1 };//右上左下方向单位矢量
	int up_left_down_right[2] = { 1,1 };//左上右下方向单位矢量
	bool forbid_three[4] = { false, false, false, false };//状态记录，分为4个方向，分别是水平，竖直，右上左下，左上右下
	bool forbid_four[4] = { false, false, false, false };//状态记录，分为4个方向，分别是水平，竖直，右上左下，左上右下
	int find_it[4] = { 0 };
	int index;
	//0代表没有连五，也没有禁手；1代表有连五；2代表有禁手

	find_it[0] = line_forbid(forbid_three, forbid_four, horizon, raw, column);
	find_it[1] = line_forbid(forbid_three, forbid_four, perpendicular, raw, column);
	find_it[2] = line_forbid(forbid_three, forbid_four, up_right_down_left, raw, column);
	find_it[3] = line_forbid(forbid_three, forbid_four, up_left_down_right, raw, column);
	
	for (index = 0; index < 4; index++)//先看看四个方向里面有没有连五
	{
		if (find_it[index] == 1)
			return false;
	}
	for (index = 0; index < 4; index++)//如果没有，再重新搜一遍，看看有没有禁手
	{
		if (find_it[index] == 2)
			return true;
	}
	return false;
}

int line_forbid(bool forbid_three[], bool forbid_four[], int vector[], int raw, int column)
{
	int dx = vector[0];
	int dy = vector[1];
	int direct_now, other_direct1, other_direct2, other_direct3;//用于帮助确定forbid方位数组的
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

	//首先检查连五
	//那么就应该是第六个位置不能是黑子。这个位置可以是白子，可以是空位，也可以是棋盘边缘
	// X●●●●?X型，X代表不是黑子
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)//连五

		&& ((board[raw - 5 * dx][column - 5 * dy] != b) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dy < 0) || (column - 5 * dy > 14))
		&& ((board[raw + 1 * dx][column + 1 * dy] != b) || (raw + 1 * dx < 0) || (raw + 1 * dx > 14) || (column + 1 * dy < 0) || (column + 1 * dy > 14))
		//最边上的位置不是黑子
		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		return 1;
	}
	// X●●●?●X型，X代表不是黑子
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)//连五

		&& ((board[raw - 4 * dx][column - 4 * dy] != b) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dy < 0) || (column - 4 * dy > 14))
		&& ((board[raw + 2 * dx][column + 2 * dy] != b) || (raw + 2 * dx < 0) || (raw + 2 * dx > 14) || (column + 2 * dy < 0) || (column + 2 * dy > 14))
		//最边上的位置不是黑子
		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		return 1;
	}

	// X●●?●●X型，X代表不是黑子

	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)

		&& ((board[raw - 3 * dx][column - 3 * dy] != b) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dy < 0) || (column - 3 * dy > 14))
		&& ((board[raw + 3 * dx][column + 3 * dy] != b) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dy < 0) || (column + 3 * dy > 14))
		//最边上的位置不是黑子

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		return 1;
	}
	
	// X●?●●●X型，X代表不是黑子
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)

		&& ((board[raw - 2 * dx][column - 2 * dy] != b) || (raw - 2 * dx < 0) || (raw - 2 * dx > 14) || (column - 2 * dy < 0) || (column - 2 * dy > 14))
		&& ((board[raw + 4 * dx][column + 4 * dy] != b) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dy < 0) || (column + 4 * dy > 14))
		//最边上的位置不是黑子

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		return 1;
	}

	//X?●●●●X型
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)

		&& ((board[raw - 1 * dx][column - 1 * dy] != b) || (raw - 1 * dx < 0) || (raw - 1 * dx > 14) || (column - 1 * dy < 0) || (column - 1 * dy > 14))
		&& ((board[raw + 5 * dx][column + 5 * dy] != b) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dy < 0) || (column + 5 * dy > 14))
		//最边上的位置不是黑子

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		return 1;
	}

	//检查长连
	//以水平方向为例
			//●●●●●?型
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw - 5 * dx][column - 5 * dy] == b)
		&& (raw - 5 * dx >= 0) && (raw - 5 * dx <= 14)
		&& (column - 5 * dy >= 0) && (column - 5 * dy <= 14))
	{
		return 2;
	}
			//●●●●?●型
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		return 2;
	}
	//●●●?●●型
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		return 2;
	}

	//●●?●●●型
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		return 2;
	}

	//●?●●●●型
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		return 2;
	}
	//?●●●●●型
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw + 5 * dx][column + 5 * dy] == b)

		&& (raw + 5 * dx >= 0) && (raw + 5 * dx <= 14)
		&& (column + 5 * dy >= 0) && (column + 5 * dy <= 14))
	{
		return 2;
	}
	//检查活四
	//写到这里了
		//水平方向
			//_?●●●_型
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] != b)
		&& (board[raw + 4 * dx][column + 4 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}
	//_●?●●_型
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//_●●?●_型
	if ((board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//_●●●?_型
	if ((board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 4 * dx][column - 4 * dy] != b)
		&& (board[raw - 4 * dx][column - 4 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//一种冲四
	//形如 ●●●_●的
	//倒过来又是一种：和 ●_●●●
	//先来上面那种
		//水平方向
			//?●●_●
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//●?●_●
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//●●?_●
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//●●●_?,这种情况只有冲四有
	if ((board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//冲四
	//●●●_●
	//倒过来又是一种：●_●●●
	//现在是下面那种

		//水平方向
			//和 ●_●●?
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//●_●?●
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}


	//●_?●●
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//?_●●●
	if ((board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//活三
	//分别是形如_●●●__
	//倒过来又是一种：__●●●_
	//先来上面那两种
		//水平方向
			//_?●●__
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& (board[raw + 4 * dx][column + 4 * dy] != b)
		&& (board[raw + 4 * dx][column + 4 * dy] != w)


		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//_●?●__
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//_●●?__
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//活三
	//分别是形如_●●●__ 
	//倒过来又是一种：__●●●_
	//现在是下面那种  需要排除这种情况__●●●__，这种就和之前的重复了。
	//还需要排除__●●●_●这个情况，这个就相当于在冲四里面再计入一个活三。这个是可能需要排除的，也可能不需要，看具体表现如何
		//水平方向
			//__●●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 4 * dx][column - 4 * dy] != b)
		&& (board[raw - 4 * dx][column - 4 * dy] != w)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)//排除__●●●_●这个情况

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	
		//这里删掉了一堆判断__●●●__的代码

	}

	//__●?●_
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)//排除__●●●_●这个情况

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}

		//这里删掉了一堆判断__●●●__的代码
	}

	//__?●●_
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& (board[raw + 4 * dx][column + 4 * dy] != b)//排除__●●●_●这个情况

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}

		//这里删掉了一堆判断__●●●__的代码
	}

	//检查冲四●●_●●Gapped22
		//水平方向
			//?●_●●
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
		
	}

	//●?_●●
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}

	}

	//●●_?●
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}

	}

	//●●_●?
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}

	}

	//检查冲四Capped_Four ○●●●●_，还有一种反方向的在下面，这里先写一种方向
		// 水平方向
			// ○?●●●_
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& ((board[raw - 1 * dx][column - 1 * dy] == w) || (raw - 1 * dx < 0) || (raw - 1 * dx > 14) || (column - 1 * dy < 0) || (column - 1 * dy > 14))
		&& (board[raw + 4 * dx][column + 4 * dy] != b)
		&& (board[raw + 4 * dx][column + 4 * dy] != w)

		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	// ○●?●●_
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& ((board[raw - 2 * dx][column - 2 * dy] == w) || (raw - 2 * dx < 0) || (raw - 2 * dx > 14) || (column - 2 * dy < 0) || (column - 2 * dy > 14))
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}
	// ○●●?●_
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] == b)
		&& ((board[raw - 3 * dx][column - 3 * dy] == w) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dy < 0) || (column - 3 * dy > 14))
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}
	// ○●●●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& ((board[raw - 4 * dx][column - 4 * dy] == w) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dy < 0) || (column - 4 * dy > 14))
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//冲四的另外一种方向_●●●●○
		// 水平方向
			// _●●●?○
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& ((board[raw + 1 * dx][column + 1 * dy] == w) || (raw + 1 * dx < 0) || (raw + 1 * dx > 14) || (column + 1 * dy < 0) || (column + 1 * dy > 14))
		&& (board[raw - 4 * dx][column - 4 * dy] != b)
		&& (board[raw - 4 * dx][column - 4 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}
	// _●●?●○
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& ((board[raw + 2 * dx][column + 2 * dy] == w) || (raw + 2 * dx < 0) || (raw + 2 * dx > 14) || (column + 2 * dy < 0) || (column + 2 * dy > 14))
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	// _●?●●○
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& ((board[raw + 3 * dx][column + 3 * dy] == w) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dy < 0) || (column + 3 * dy > 14))
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	// _?●●●○
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& ((board[raw + 4 * dx][column + 4 * dy] == w) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dy < 0) || (column + 4 * dy > 14))
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_four[direct_now] = true;
		if (forbid_four[other_direct1] || forbid_four[other_direct2] || forbid_four[other_direct3])
		{
			return 2;
		}
	}

	//检查扁担四：
	//X●●_●●_●●X & X●●●_●_●●●X & X●_●●●_●X
	//注意打X的位点不能是自己的子，否则那几个夹在中间的空位会产生长连禁手而不能落子
	//注意搜索位点只在中间部分，边上的那几个子不要搜
	//先检查X●●_●●_●●X
	//X●●_?●_●●X
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& ((board[raw + 5 * dx][column + 5 * dy] != b) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dx < 0) || (column + 5 * dx > 14))
		&& ((board[raw - 4 * dx][column - 4 * dy] != b) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dx < 0) || (column - 4 * dx > 14))

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		return 2;
	}
	//X●●_●?_●●X
	if ((board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& ((board[raw + 4 * dx][column + 4 * dy] != b) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dx < 0) || (column + 4 * dx > 14))
		&& ((board[raw - 5 * dx][column - 5 * dy] != b) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dx < 0) || (column - 5 * dx > 14))

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		return 2;
	}
	//检查X●●●_●_●●●X
	//X●●●_?_●●●X
	if ((board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& ((board[raw + 5 * dx][column + 5 * dy] != b) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dx < 0) || (column + 5 * dx > 14))
		&& ((board[raw - 5 * dx][column - 5 * dy] != b) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dx < 0) || (column - 5 * dx > 14))

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		return 2;
	}

	//检查X●_●●●_●X
	//X●_?●●_●X
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 4 * dx][column + 4 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& ((board[raw + 5 * dx][column + 5 * dy] != b) || (raw + 5 * dx < 0) || (raw + 5 * dx > 14) || (column + 5 * dx < 0) || (column + 5 * dx > 14))
		&& ((board[raw - 3 * dx][column - 3 * dy] != b) || (raw - 3 * dx < 0) || (raw - 3 * dx > 14) || (column - 3 * dx < 0) || (column - 3 * dx > 14))

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		return 2;
	}

	//X●_●?●_●X
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& ((board[raw + 4 * dx][column + 4 * dy] != b) || (raw + 4 * dx < 0) || (raw + 4 * dx > 14) || (column + 4 * dx < 0) || (column + 4 * dx > 14))
		&& ((board[raw - 4 * dx][column - 4 * dy] != b) || (raw - 4 * dx < 0) || (raw - 4 * dx > 14) || (column - 4 * dx < 0) || (column - 4 * dx > 14))

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		return 2;
	}

	//X●_●●?_●X
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 4 * dx][column - 4 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)
		&& ((board[raw + 3 * dx][column + 3 * dy] != b) || (raw + 3 * dx < 0) || (raw + 3 * dx > 14) || (column + 3 * dx < 0) || (column + 3 * dx > 14))
		&& ((board[raw - 5 * dx][column - 5 * dy] != b) || (raw - 5 * dx < 0) || (raw - 5 * dx > 14) || (column - 5 * dx < 0) || (column - 5 * dx > 14))

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		return 2;
	}

	//检查跳活三Gapped_Three  _●_●●_与_●●_●_
		//要排除_●_●●_●_这种情况
		//水平方向
		//先检查_●_●●_
			//_?_●●_
	if ((board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw + 4 * dx][column + 4 * dy] != b)
		&& (board[raw + 4 * dx][column + 4 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//_●_?●_
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 3 * dx][column - 3 * dy] != b)
		&& (board[raw - 3 * dx][column - 3 * dy] != w)

		&& (raw - 3 * dx >= 0) && (raw - 3 * dx <= 14)
		&& (column - 3 * dy >= 0) && (column - 3 * dy <= 14)
		&& (raw + 2 * dx >= 0) && (raw + 2 * dx <= 14)
		&& (column + 2 * dy >= 0) && (column + 2 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//_●_●?_
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 4 * dx][column - 4 * dy] != b)
		&& (board[raw - 4 * dx][column - 4 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//再检查_●●_●_
		//_●●_?_
	if ((board[raw - 2 * dx][column - 2 * dy] == b)
		&& (board[raw - 3 * dx][column - 3 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& (board[raw - 4 * dx][column - 4 * dy] != b)
		&& (board[raw - 4 * dx][column - 4 * dy] != w)

		&& (raw - 4 * dx >= 0) && (raw - 4 * dx <= 14)
		&& (column - 4 * dy >= 0) && (column - 4 * dy <= 14)
		&& (raw + 1 * dx >= 0) && (raw + 1 * dx <= 14)
		&& (column + 1 * dy >= 0) && (column + 1 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//_●?_●_
	if ((board[raw - 1 * dx][column - 1 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] == b)
		&& (board[raw + 1 * dx][column + 1 * dy] != b)
		&& (board[raw + 1 * dx][column + 1 * dy] != w)
		&& (board[raw - 2 * dx][column - 2 * dy] != b)
		&& (board[raw - 2 * dx][column - 2 * dy] != w)
		&& (board[raw + 3 * dx][column + 3 * dy] != b)
		&& (board[raw + 3 * dx][column + 3 * dy] != w)

		&& (raw - 2 * dx >= 0) && (raw - 2 * dx <= 14)
		&& (column - 2 * dy >= 0) && (column - 2 * dy <= 14)
		&& (raw + 3 * dx >= 0) && (raw + 3 * dx <= 14)
		&& (column + 3 * dy >= 0) && (column + 3 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}

	//_?●_●_
	if ((board[raw + 1 * dx][column + 1 * dy] == b)
		&& (board[raw + 3 * dx][column + 3 * dy] == b)
		&& (board[raw + 2 * dx][column + 2 * dy] != b)
		&& (board[raw + 2 * dx][column + 2 * dy] != w)
		&& (board[raw - 1 * dx][column - 1 * dy] != b)
		&& (board[raw - 1 * dx][column - 1 * dy] != w)
		&& (board[raw + 4 * dx][column + 4 * dy] != b)
		&& (board[raw + 4 * dx][column + 4 * dy] != w)

		&& (raw - 1 * dx >= 0) && (raw - 1 * dx <= 14)
		&& (column - 1 * dy >= 0) && (column - 1 * dy <= 14)
		&& (raw + 4 * dx >= 0) && (raw + 4 * dx <= 14)
		&& (column + 4 * dy >= 0) && (column + 4 * dy <= 14))
	{
		forbid_three[direct_now] = true;
		if (forbid_three[other_direct1] || forbid_three[other_direct2] || forbid_three[other_direct3])
		{
			return 2;
		}
	}


	return 0;
}