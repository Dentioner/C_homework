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
extern int w;//白棋
extern int b;//黑棋
extern unsigned long long ZobristTable[15][15][2];//梅森旋转的哈希键值表
extern unsigned long long hashValue;//梅森旋转算法下，棋盘的哈希值
extern unsigned long long hashing_value2[depth_of_hashing][3];

long int best_score_of_upper[11] = { -infinity , infinity , -infinity, infinity , -infinity, infinity, -infinity , infinity , -infinity, infinity, -infinity };//给minimax里面的剪枝用的
//注意上下这两个数组的编号问题。floor是从11往0递归的，因此要保持最后一个元素不变。
bool not_in_the_same_branch[11] = { true, true, true, true, true, true, true, true, true, true, true };

long int best_score_of_upper_ver2[12] = { infinity ,-infinity , infinity , -infinity, infinity , -infinity, infinity, -infinity , infinity , -infinity, infinity, -infinity };//给minimax里面的剪枝用的
long int value_for_board = 0;//新加
bool ai_first = false;//默认电脑后走

void pve(long int value)
{
	//PVE

	int ai_choice = 0;
	int floor = FLOOR;//搜索层数
	int chess;
	int opponent_chess;
	int step_count = 0; //游戏下了几个子的计数

	bool ai_first = false;//默认电脑后走
	bool my_turn = true; //这个东西是确认这一步是哪一方下子了
	bool continue_playing = true; //确认游戏是否继续


	initTable();
	
	ai_choice = offensive();

	if (ai_choice == 1)
	{
		ai_first = true;
	}
	else
	{
		ai_first = false;
	}

	if (ai_first)
	{
		my_turn = true;
		chess = b;
		opponent_chess = w;
		board[7][7] = chess;
		my_turn = false;
		step_count++;
	}
	else
	{
		my_turn = false;
		chess = w;
		opponent_chess = b;

	}
	DrawBoard(value, 2,  -1);
	//initialize_hashing_sheet(key);
	while (continue_playing)
	{

		//注意这里无需将my_turn求反了，在αβ剪枝函数中已经做了
		//注意，这里删除了priority数组，要看的话去1.0版本
		//还删除了杀棋用的那几个数组
		
		if (my_turn)
		{
			double start_time = clock();
			double end_time, cost_time;
			if (step_count > 2)
			{
				value = Minimax2(step_count, my_turn, ai_first, floor);
				if ((coordinate[0] == 0) && (coordinate[1] == 1))
				{
					auto_play(chess, opponent_chess);
					chess_play_ver2(step_count);
					hashValue ^= ZobristTable[coordinate[0]][coordinate[1]][(step_count % 2)];
					DrawBoard(value, 2, step_count);
					return_to_normal_chess(step_count);
				}
				else
				{
					chess_play_ver2(step_count);
					hashValue ^= ZobristTable[coordinate[0]][coordinate[1]][(step_count % 2)];
					DrawBoard(value, 2, step_count);
					return_to_normal_chess(step_count);
				}

				//测试，打印哈希表，看看表是否正常
				/*if (coordinate[0] == 8 && coordinate[1] == 7)
				{
					for (int ha1 = 0; ha1 < depth_of_hashing; ha1++)
					{
						if (hashing_value2[ha1][0] != 0)
						{


							printf("棋盘：%llu，我方：%lld，对方：%lld\n", hashing_value2[ha1][0], hashing_value2[ha1][1], hashing_value2[ha1][2]);
							//第二维度的[0]是整个棋盘的哈希值
							//[1]与[2]是此哈希值对应的棋盘评分，应该定义在函数外面;
						}
					}
					printf("本语句是方便打断点设置的\n");
				}
				*/
			}
			else
			{
				auto_play(chess, opponent_chess);
				chess_play_ver2(step_count);
				hashValue ^= ZobristTable[coordinate[0]][coordinate[1]][(step_count % 2)];
				DrawBoard(value, 2, step_count);
				return_to_normal_chess(step_count);
			}
			end_time = clock();
			cost_time = (end_time - start_time) / CLK_TCK;
			printf("time=%fs.\n", cost_time);

		}
		else
		{
			get_coordinate(step_count);
			//把value和chessplay换过位置了，不知道会怎样
			roaming = board[coordinate[0]][coordinate[1]];//记录上一步的状态
			value = evaluation(step_count, my_turn, coordinate[0], coordinate[1]);
			chess_play_ver2(step_count);
			hashValue ^= ZobristTable[coordinate[0]][coordinate[1]][(step_count % 2)];
			DrawBoard(value, 2, step_count);
			return_to_normal_chess(step_count);
			//下面是悔棋代码，可能会有问题			
			printf("是否想要悔棋？按y悔棋，按别的任意键正常继续游戏.\n");
			char c_getback = ' ';
			c_getback = getchar();
			while (getchar() != '\n')
				continue;
			int i_getback = c_getback;
			if (i_getback == 89 || i_getback == 121)
			{
				board[coordinate[0]][coordinate[1]] = roaming;
				hashValue ^= ZobristTable[coordinate[0]][coordinate[1]][(step_count % 2)];
				DrawBoard(value, 2, step_count);
				continue;
			}

		}

		continue_playing = judgement(step_count);
		my_turn = !my_turn;
		step_count++;
	}
	DrawBoard(value, 2, step_count);
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

int offensive()//选择谁先手谁后手
{
	bool invalid_ai_choice = true;
	int ai_choice_index = 0;
	int ai_choice = 0;

	printf("电脑先手还是对方先手？\n");
	printf("1.电脑先手\t\t2.对方先手\n");
	printf("************************************************\n");

	while (invalid_ai_choice)
	{
		ai_choice_index = scanf("%d", &ai_choice);
		while (getchar() != '\n')
			continue;
		if (ai_choice_index != 1)
		{
			printf("无效输入，请重试\n");

			continue;
		}
		if ((ai_choice != 1) && (ai_choice != 2))
		{
			printf("无效输入，请重试\n");

			continue;
		}

		invalid_ai_choice = false;
	}


	return ai_choice;
}

void auto_play(int chess, int opponent_chess)
{


	for (int p = 0; p < 15; p++)
	{
		for (int q = 0; q < 15; q++)
		{
			if (board[p][q] == opponent_chess)
			{
				if ((board[p + 1][q] != opponent_chess)
					&& (board[p + 1][q] != chess)
					&& (p + 1 <= 14))
				{
					coordinate[0] = p + 1;
					coordinate[1] = q;
					
					return;
				}

				else if ((board[p][q + 1] != opponent_chess)
					&& (board[p][q + 1] != chess)
					&& (q + 1 <= 14))
				{
					coordinate[0] = p;
					coordinate[1] = q + 1;
					
					return;
				}
				else if ((board[p - 1][q] != opponent_chess)
					&& (board[p - 1][q] != chess)
					&& (p - 1 >= 0))
				{
					coordinate[0] = p - 1;
					coordinate[1] = q;
					
					return;
				}
				else if ((board[p][q - 1] != opponent_chess)
					&& (board[p][q - 1] != chess)
					&& (q - 1 >= 0))
				{
					coordinate[0] = p;
					coordinate[1] = q - 1;
					//strncpy(board[p][q - 1], chess, 2);
					return;
				}
				else if ((board[p - 1][q - 1] != opponent_chess)
					&& (board[p - 1][q - 1] != chess)
					&& (p - 1 >= 0)
					&& (q - 1 >= 0))
				{
					coordinate[0] = p - 1;
					coordinate[1] = q - 1;
					return;
				}
				else if ((board[p - 1][q + 1] != opponent_chess)
					&& (board[p - 1][q + 1] != chess)
					&& (p - 1 >= 0)
					&& (q + 1 <= 14))
				{
					coordinate[0] = p - 1;
					coordinate[1] = q + 1;
					return;
				}
				else if ((board[p + 1][q + 1] != opponent_chess)
					&& (board[p + 1][q + 1] != chess)
					&& (p + 1 <= 14)
					&& (q + 1 <= 14))
				{
					coordinate[0] = p + 1;
					coordinate[1] = q + 1;
					
					return;
				}
				else if ((board[p + 1][q - 1] != opponent_chess)
					&& (board[p + 1][q - 1] != chess)
					&& (p + 1 <= 14)
					&& (q - 1 >= 0))
				{
					coordinate[0] = p + 1;
					coordinate[1] = q - 1;
					
					return;
				}

			}
		}
	}

}

void init_best_score_of_upper()
{
	//best_score_of_upper_ver2[12] = { infinity ,-infinity , infinity , -infinity, infinity , -infinity, infinity, -infinity , infinity , -infinity, infinity, -infinity };//给minimax里面的剪枝用的
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		if (i % 2 == 0)
		{
			best_score_of_upper_ver2[i] = infinity;
		}
		else
		{
			best_score_of_upper_ver2[i] = -infinity;
		}
	}
}