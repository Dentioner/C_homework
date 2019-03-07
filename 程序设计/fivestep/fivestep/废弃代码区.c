#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include"head.h"
/*
//注意写好之后要按照老师给的规范进行优化，见五子棋规则的doc文件
void main()
{
	int step_count = 0; //游戏下了几个子的计数
	bool my_turn = true; //暂时用不上，这个东西是确认这一步是哪一方下子了
	bool continue_playing = true; //确认游戏是否继续
	bool ai_first = false;//默认电脑后走
	bool invalid_mode = true;
	long int value = 0;//评分函数的打分
	long int my_value = 0;//我方得分
	long int opponent_value = 0;//对方得分
	int mode_choice;
	int mode_choice_index;
	char board[15][17][2] =
	{
		{"15","┏","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┓","15"},
		{"14","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","14"},
		{"13","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","13"},
		{"12","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","12"},
		{"11","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","11"},
		{"10","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","10"},
		{"9","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","9"},
		{"8","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","8"},
		{"7","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","7"},
		{"6","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","6"},
		{"5","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","5"},
		{"4","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","4"},
		{"3","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","3"},
		{"2","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","2"},
		{"1","┗","┷","┷","┷","┷","┷","┷","┷","┷","┷","┷","┷","┷","┷","┛","1"}

	};

	int coordinate[2] = { 0, 0 };
	char black[2] = "○";
	char white[2] = "●";
	//这里准备写一个判断是PVP还是PVE的语句
		//如果是PVE，选择黑子还是白子
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

	if (mode_choice == 1)
	{

		while (continue_playing)
		{
			//在这个循环里面试着将评分函数混进去
			//首先尝试着将自己下的每一步用评分函数打个分吧
			DrawBoard(board, 15, value, mode_choice);
			//chess_play(board, step_count);老的chessplay函数
			get_coordinate(coordinate, board, step_count);
			chess_play_ver2(board, step_count, coordinate);
			value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
			//my_value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
			//opponent_value = evaluation(board, step_count + 1, !my_turn, coordinate[0], coordinate[1]);
			//value = my_value + opponent_value;
			//上面这个分开打分的可能有问题
			continue_playing = judgement(board, step_count);
			my_turn = !my_turn;
			step_count++;
		}
		DrawBoard(board, 15, value, mode_choice);
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


	else
	{
		//PVE

		printf("电脑先手还是对方先手？\n");
		printf("1.电脑先手\t\t2.对方先手\n");
		printf("************************************************\n");
		int ai_choice = 0;
		int ai_choice_index = 0;
		int floor = FLOOR;//搜索层数
		char *chess;
		char *opponent_chess;
		bool invalid_ai_choice = true;
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
			chess = black;
			opponent_chess = white;
			strncpy(board[8][7], chess, 2);
		}
		else
		{
			my_turn = false;
			chess = white;
			opponent_chess = black;

		}
		while (continue_playing)
		{
			DrawBoard(board, 15, value, mode_choice);
			//注意这里无需将my_turn求反了，在αβ剪枝函数中已经做了
			if (my_turn)
			{

				if (step_count > 4)
				{
					value = Minimax2(board, step_count, my_turn, ai_first, floor, coordinate);
					chess_play_ver2(board, step_count, coordinate);
				}
				else
				{
					for (int p = 0; p < 15; p++)
					{
						for (int q = 0; q < 17; q++)
						{
							if ((strncmp(board[p][q], opponent_chess, 2)) == 0)
							{
								if (((strncmp(board[p + 1][q], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p + 1][q], chess, 2)) != 0))
								{
									strncpy(board[p + 1][q], chess, 2);
								}

								else if (((strncmp(board[p][q + 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p][q + 1], chess, 2)) != 0))
								{
									strncpy(board[p][q + 1], chess, 2);
								}
								else if (((strncmp(board[p - 1][q], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p - 1][q], chess, 2)) != 0))
								{
									strncpy(board[p - 1][q], chess, 2);
								}
								else if (((strncmp(board[p][q - 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p][q - 1], chess, 2)) != 0))
								{
									strncpy(board[p][q - 1], chess, 2);
								}
								else if (((strncmp(board[p - 1][q - 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p - 1][q - 1], chess, 2)) != 0))
								{
									strncpy(board[p - 1][q - 1], chess, 2);
								}
								else if (((strncmp(board[p - 1][q + 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p - 1][q + 1], chess, 2)) != 0))
								{
									strncpy(board[p - 1][q + 1], chess, 2);
								}
								else if (((strncmp(board[p + 1][q + 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p + 1][q + 1], chess, 2)) != 0))
								{
									strncpy(board[p + 1][q + 1], chess, 2);
								}
								else if (((strncmp(board[p + 1][q - 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p + 1][q - 1], chess, 2)) != 0))
								{
									strncpy(board[p + 1][q - 1], chess, 2);
								}

							}
						}
					}
				}
			}
			else
			{
				get_coordinate(coordinate, board, step_count);
				chess_play_ver2(board, step_count, coordinate);
				value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
			}

			continue_playing = judgement(board, step_count);
			my_turn = !my_turn;
			step_count++;
		}
		DrawBoard(board, 15, value, mode_choice);
		if (step_count % 2)
		{
			printf("黑子获胜");
		}
		else
		{
			printf("白子获胜");
		}

		return;


		printf("施工中\n");

	}

}

void DrawBoard(char board[][17][2], int i, long int value, int mode_choice)
{
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



	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			for (int k = 0; k < 2; k++)
				putchar(board[i][j][k]);
		}
		putchar('\n');
	}
	printf("   A B C D E F G H I J K L M N O \n");
	//system("pause");
	printf("value = %ld\n", value);

}



*/

/*
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"head.h"
//启发式搜索，按照优先级顺序，对分支进行排序，提高剪枝速度
//见https://github.com/lihongxun945/gobang/blob/master/src/ai/board.js
bool before_evaluation(char board[][17][2], int priority[][10][2], int floor,
	int step_count, bool my_turn)
	//step_count及其后面的几个形参是用来给evaluation的
{
	int my_raw = 0;
	int my_column = 0;
	char black[2] = "○";
	char white[2] = "●";
	long int my_value = 0;
	long int opponent_value = 0;
	bool get_coordinate1 = false;
	bool get_coordinate2 = false;
	bool get_coordinate3 = false;
	bool get_coordinate4 = false;
	bool get_coordinate5 = false;
	bool get_coordinate6 = false;
	bool get_coordinate7 = false;
	bool get_coordinate8 = false;
	bool get_coordinate9 = false;
	bool get_coordinate10 = false;
	bool final_hit = false;//发现我方连五直接返回
	for (my_raw = 0; my_raw < 15; my_raw++)
	{
		for (my_column = 1; my_column < 16; my_column++)
		{

			if ((strncmp(board[my_raw][my_column], black, 2) != 0)
				&& (strncmp(board[my_raw][my_column], white, 2) != 0))
			{
				my_value = evaluation(board, step_count, my_turn, my_raw, my_column);
				opponent_value = evaluation(board, step_count + 1, !my_turn, my_raw, my_column);
				if (my_value >= Consecutive_Five)//如果发现了我方连五的地方
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						final_hit = true;
						return final_hit;
					}
					final_hit = true;
					return final_hit;

				}
				else if (opponent_value <= -Consecutive_Five)//如果发现了对方连五的地方
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (my_value >= Open_Four)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (opponent_value <= -Open_Four)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (my_value >= 2 * Open_Three)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (opponent_value <= -2 * Open_Three)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (my_value >= Capped_Four)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (opponent_value <= -Capped_Four)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (my_value >= Open_Three)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}
				else if (opponent_value <= -Open_Three)
				{
					if (!get_coordinate1)//如果第一个位置是空的
					{

						priority[FLOOR - floor][0][0] = my_raw;
						priority[FLOOR - floor][0][1] = my_column;
						get_coordinate1 = true;
					}
					else if (!get_coordinate2)//如果第二个位置是空的
					{

						priority[FLOOR - floor][1][0] = my_raw;
						priority[FLOOR - floor][1][1] = my_column;
						get_coordinate2 = true;
					}
					else if (!get_coordinate3)//如果第三个位置是空的
					{

						priority[FLOOR - floor][2][0] = my_raw;
						priority[FLOOR - floor][2][1] = my_column;
						get_coordinate3 = true;
					}
					else if (!get_coordinate4)//如果第四个位置是空的
					{

						priority[FLOOR - floor][3][0] = my_raw;
						priority[FLOOR - floor][3][1] = my_column;
						get_coordinate4 = true;
					}
					else if (!get_coordinate5)//如果第五个位置是空的
					{

						priority[FLOOR - floor][4][0] = my_raw;
						priority[FLOOR - floor][4][1] = my_column;
						get_coordinate5 = true;
					}
					else if (!get_coordinate6)//如果第六个位置是空的
					{

						priority[FLOOR - floor][5][0] = my_raw;
						priority[FLOOR - floor][5][1] = my_column;
						get_coordinate6 = true;
					}
					else if (!get_coordinate7)//如果第七个位置是空的
					{

						priority[FLOOR - floor][6][0] = my_raw;
						priority[FLOOR - floor][6][1] = my_column;
						get_coordinate7 = true;
					}
					else if (!get_coordinate8)//如果第八个位置是空的
					{

						priority[FLOOR - floor][7][0] = my_raw;
						priority[FLOOR - floor][7][1] = my_column;
						get_coordinate8 = true;
					}
					else if (!get_coordinate9)//如果第九个位置是空的
					{

						priority[FLOOR - floor][8][0] = my_raw;
						priority[FLOOR - floor][8][1] = my_column;
						get_coordinate9 = true;
					}
					else if (!get_coordinate10)//如果第十个位置是空的
					{

						priority[FLOOR - floor][9][0] = my_raw;
						priority[FLOOR - floor][9][1] = my_column;
						get_coordinate10 = true;
						return final_hit;
					}
				}

			}

			if (get_coordinate1
				&& get_coordinate2
				&& get_coordinate3
				&& get_coordinate4
				&& get_coordinate5
				&& get_coordinate6
				&& get_coordinate7
				&& get_coordinate8
				&& get_coordinate9
				&& get_coordinate10)
			{
				return final_hit;
			}

		}




	}
	return final_hit;
}
*/


/*
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"head.h"


/*
下面是伪代码
先遍历一遍棋盘，给每个区域打分
宽度设置为3，也就是搜索最大分数的前三个
	第一层：先把那个棋下下去，在最后再将棋子还原即可，或者创建一个副本temp，在temp里面修改
	下下去之后，从对手的角度看，也就是找最小值，同样找3个点
	然后再迭代，又找最大值
	顺序：


//PVE的chess由my_turn决定（暂定）

int Minimax(char board[][17][2], int step_count,
	bool my_turn, bool ai_first, int floor,
	int child_coordinate[], int coordinate[])//coordinate由board.c定义
{//floor为搜索层数,main里面设定为FLOOR的值，目前为3
//递归的最外层需要返回一组坐标coordinate，其余的层数不要返回
	//child_coordinate[]是否还需要？
	long int temp = 0;
	long int max[3] = { 0,0,0 };
	long int min[3] = { 0,0,0 };
	int max_coordinate[3][2] = { {0,0},{0,0},{0,0} };
	int min_coordinate[3][2] = { {0,0},{0,0},{0,0} };
	int child_coordinate1[2] = { 0,0 };
	int child_coordinate2[2] = { 0,0 };
	int child_coordinate3[2] = { 0,0 };//3个子节点的坐标，可能是max，可能是min
	int* best_coordinate;//返回出去的坐标指针
	char black[2] = "○";
	char white[2] = "●";
	char temp_blank[2];//用这个来还原棋盘，相当于悔棋一样的
	char *chess;
	char *opponent_chess;
	//下面是在建立ai先手、回合数与“是否是我方回合”的关系
	if (ai_first)
	{
		chess = black;
		opponent_chess = white;
		if ((step_count % 2) == 0)//如果step数不能整除2的话，就是白子走
		{
			my_turn = true;
		}
		else
		{
			my_turn = false;
		}
	}
	else
	{
		chess = white;
		opponent_chess = black;
		if ((step_count % 2) == 0)
		{
			my_turn = false;
		}
		else
		{
			my_turn = true;
		}
	}

	strncpy(temp_blank,
		board[child_coordinate[0]][child_coordinate[1]],
		2);//将空白处先储存起来，之后再还原

	strncpy(board[child_coordinate[0]][child_coordinate[1]],
		chess, 2);//假如说上一层觉得这个点最好，在这个点下了棋的基础上研究下面的东西
	//这个if语句是在遍历一遍棋盘，给每个区域打分，搜索最大/最小分数的前三个
	if (my_turn)
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				temp = evaluation(board, step_count, my_turn, i, j);
				if ((temp > max[2]) && (temp <= max[1]) && (temp <= max[0]))
				{
					max[2] = temp;
					max_coordinate[2][0] = i;
					max_coordinate[2][1] = j;
				}
				else if ((temp > max[2]) && (temp > max[1]) && (temp <= max[0]))
				{
					max[2] = max[1];
					max_coordinate[2][0] = max_coordinate[1][0];
					max_coordinate[2][1] = max_coordinate[1][0];

					max[1] = temp;
					max_coordinate[1][0] = i;
					max_coordinate[1][1] = j;
				}
				else if((temp > max[2]) && (temp > max[1]) && (temp > max[0]))
				{
					max[2] = max[1];
					max_coordinate[2][0] = max_coordinate[1][0];
					max_coordinate[2][1] = max_coordinate[1][0];
					max[1] = max[0];
					max_coordinate[1][0] = max_coordinate[0][0];
					max_coordinate[1][1] = max_coordinate[0][0];
					max[0] = temp;
					max_coordinate[0][0] = i;
					max_coordinate[0][1] = j;
				}
				else
				{
					;
				}

			}
		}

	}

	else
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				temp = evaluation(board, step_count, my_turn, i, j);
				if ((temp < min[2]) && (temp >= min[1]) && (temp >= min[0]))
				{
					min[2] = temp;
					min_coordinate[2][0] = i;
					min_coordinate[2][1] = j;
				}
				else if ((temp < min[2]) && (temp < min[1]) && (temp >= min[0]))
				{
					min[2] = min[1];
					min_coordinate[2][0] = min_coordinate[1][0];
					min_coordinate[2][1] = min_coordinate[1][1];
					min[1] = temp;
					min_coordinate[1][0] = i;
					min_coordinate[1][1] = j;
				}
				else if ((temp < min[2]) && (temp < min[1]) && (temp < min[0]))
				{
					min[2] = min[1];
					min_coordinate[2][0] = min_coordinate[1][0];
					min_coordinate[2][1] = min_coordinate[1][1];
					min[1] = min[0];
					min_coordinate[1][0] = min_coordinate[0][0];
					min_coordinate[1][1] = min_coordinate[0][1];
					min[0] = temp;
					min_coordinate[0][0] = i;
					min_coordinate[0][1] = j;
				}
				else
				{
					;
				}

			}
		}
	}

	if (((FLOOR - floor) % 2) == 0)//如果层数为偶数，子节点就是最大的三个坐标点
	{
		child_coordinate1[0] = max_coordinate[0][0];
		child_coordinate1[1] = max_coordinate[0][1];
		child_coordinate2[0] = max_coordinate[1][0];
		child_coordinate2[1] = max_coordinate[1][1];
		child_coordinate3[0] = max_coordinate[2][0];
		child_coordinate3[1] = max_coordinate[2][1];
	}
	else
	{
		child_coordinate1[0] = min_coordinate[0][0];
		child_coordinate1[1] = min_coordinate[0][1];
		child_coordinate2[0] = min_coordinate[1][0];
		child_coordinate2[1] = min_coordinate[1][1];
		child_coordinate3[0] = min_coordinate[2][0];
		child_coordinate3[1] = min_coordinate[2][1];
	}
//至此，获得了三个节点的信息
//下面开始递归
	//递归规则待完善
	//传递的坐标是否是指针？
	/*顺序：
第0层找三个坐标→给第1层→第1层找3个坐标→给第2层→第二层找3个坐标→
→第二层给3个坐标打分→给第1层最大值/最小值的坐标→第1层获得最大值/最小值的坐标后打分
→给第0层最大值/最小值的坐标→第0层打分→返回给外面一个最优的坐标
在这个过程中还要下棋，下完棋之后还得将棋盘变回去

	printf("floor = %d\nFLOOR - floor = %d\n.", floor, FLOOR - floor);

	if (floor != 0)//还没到最底层的话就递归
	{
/*
		Minimax(board, step_count + 1, !my_turn,
				ai_first, floor - 1, child_coordinate1);//这3个就是递归表达式
		Minimax(board, step_count + 1, !my_turn,
			ai_first, floor - 1, child_coordinate2);
		Minimax(board, step_count + 1, !my_turn,
			ai_first, floor - 1, child_coordinate3);



	}


	//到了最底层之后,以及递归回来以后，就要找最值，向上传递指针best_coordinate了



	//递归函数重写，参考书P257
	/*
	if (floor != 0)//只要不是最里面一层
	{
		Minimax(board, step_count + 1, !my_turn, ai_first, floor - 1, coordinate);
		if (((FLOOR - floor) % 2) == 0)//如果搜索层数是偶数的话
		{
			*coordinate = **max_coordinate;
		}
		else
		{
			*coordinate = **min_coordinate;
		}
	}
	else//最底层
	{
		if (((FLOOR - floor) % 2) == 0)//如果搜索层数是偶数的话
		{
			*coordinate = **max_coordinate;
		}
		else
		{
			*coordinate = **min_coordinate;
		}
	}


	strncpy(board[child_coordinate[0]][child_coordinate[1]],
		temp_blank, 2);//将棋盘复原

}


long int Minimax2(char board[][17][2], int step_count,
	bool my_turn, bool ai_first,
	int floor, int coordinate[], long int best_score_of_upper[], int priority[][26][2])
{
	char black[2] = "○";
	char white[2] = "●";
	char temp_blank[2];//用这个来还原棋盘，相当于悔棋一样的
	char *chess;
	char *opponent_chess;
	bool final_hit = false;
	//bool valid_coordinate = false;
	//下面是在建立ai先手、回合数与“是否是我方回合”的关系

	if ((step_count % 2) == 0)
	{
		chess = black;
		opponent_chess = white;
		if (ai_first)
		{
			my_turn = true;
		}
		else
		{
			my_turn = false;
		}
	}
	else
	{
		chess = white;
		opponent_chess = black;
		if (ai_first)
		{
			my_turn = false;
		}
		else
		{
			my_turn = true;
		}

	}
	long int best_score = 0;
	int best_coordinate[2] = { 0,1 };
	long int temp_score = 0;
	
	//找最佳的点与最值
	if (my_turn)
	{
		temp_score = 0;
		//在设计优先级函数之前，先如下凑合着用
		//raw和column的搜索范围缩小一点，这样可以减少运算量，棋盘边缘就不搜索了
		for (int raw = 3; raw < 12; raw++)
		{
			for (int column = 3; column < 14; column++)
			{
				temp_score = evaluation(board, step_count, my_turn, raw, column);
				if (temp_score > best_score)
				{
					best_score = temp_score;
					best_coordinate[0] = raw;
					best_coordinate[1] = column;
				}
			}
		}
	}
	else
	{
		temp_score = 0;
		for (int raw = 0; raw < 15; raw++)
		{
			for (int column = 0; column < 17; column++)
			{
				temp_score = evaluation(board, step_count, my_turn, raw, column);
				if (temp_score < best_score)
				{
					best_score = temp_score;
					best_coordinate[0] = raw;
					best_coordinate[1] = column;
				}
			}
		}
	}

	strncpy(temp_blank, board[best_coordinate[0]][best_coordinate[1]], 2);
	strncpy(board[best_coordinate[0]][best_coordinate[1]], chess, 2);
	//下面开始递归
	
	//printf("floor = %d\nFLOOR - floor = %d\n.", floor, FLOOR - floor);
	//不是最底层↓


	if (floor != 0)
	{
		if (my_turn)
		{
			//best_score_of_upper[floor] = 0;
			//先将优先的那些点找到并递归
			final_hit = before_evaluation(board, priority, floor, step_count, my_turn);
			for (int test_raw = 0; test_raw < 4; test_raw++)
			{
				for (int test_raw2 = 0; test_raw2 < 26; test_raw2++)
				{

					printf("%d,", priority[test_raw][test_raw2][0]);
					printf("%d", priority[test_raw][test_raw2][1]);
					printf(" ");
				}
				printf("\n");
			}


			if (final_hit
				&& (FLOOR == floor))
			{//如果发现有连五，直接秒了
				coordinate[0] = priority[0][0][0];
				coordinate[1] = priority[0][0][1];
				best_score = evaluation(board, step_count, my_turn, priority[0][0][0], priority[0][0][1]);
				return best_score;
			}
			if (!final_hit && (FLOOR == floor))
			{//如果发现对方马上形成连五，但是自己没有连五，赶紧堵上去
				if ((priority[0][1][0] != 0) || (priority[0][1][1]) != 0)
				{
					coordinate[0] = priority[0][1][0];
					coordinate[1] = priority[0][1][1];
					best_score = evaluation(board, step_count, my_turn, priority[0][1][0], priority[0][1][1]);
					return best_score;
				}

			}
			for (int a = 0; a < 26; a++)
			{
				int raw = priority[FLOOR - floor][a][0];
				int column = priority[FLOOR - floor][a][1];
				if ((raw != 0) || (column != 0))
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						strncpy(temp_blank, board[raw][column], 2);
						strncpy(board[raw][column], chess, 2);
						DrawBoard(board, 15, temp_score, 2);
						temp_score = Minimax2(board, step_count + 1,
							!my_turn, ai_first,
							floor - 1, coordinate, best_score_of_upper, priority);

						//DrawBoard(board, 15, temp_score, 2);
						if ((temp_score != 0) && (best_score == 0))
						{
							best_score = temp_score;

						}
						if (temp_score > best_score)
						{
							best_score = temp_score;

							//valid_coordinate = verify_coordinate(board, raw, column, chess, opponent_chess);
							//if ((floor == FLOOR)&&valid_coordinate)
							if (floor == FLOOR)
								//如果是最外层，记录此时坐标
							{
								best_coordinate[0] = raw;
								best_coordinate[1] = column;
							}
							//这个剪枝待修改
							else
							{
								if (best_score > best_score_of_upper[floor])//剪枝
								{
									strncpy(board[raw][column], temp_blank, 2);
									return 89999900;
								}
							}
						}
						//复原
						strncpy(board[raw][column], temp_blank, 2);
						if (best_score < best_score_of_upper[floor - 1])
						{
							best_score_of_upper[floor - 1] = best_score;
						}

					}
				}




			}
			//这行下面全遍历棋盘的循环先废弃了
			
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 1; column < 16; column++)
				{//每一个点落子，产生子节点，因此实际上每一个点都会产生255个子节点
					//↓
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						strncpy(temp_blank, board[raw][column], 2);
						strncpy(board[raw][column], chess, 2);
					//	DrawBoard(board, 15, temp_score, 2);
						temp_score = Minimax2(board, step_count + 1,
							!my_turn, ai_first,
							floor - 1, coordinate, best_score_of_upper);

							//DrawBoard(board, 15, temp_score, 2);
						if ((temp_score != 0)&&(best_score == 0))
						{
							best_score = temp_score;

						}
						if (temp_score > best_score)
						{
							best_score = temp_score;

							//valid_coordinate = verify_coordinate(board, raw, column, chess, opponent_chess);
							//if ((floor == FLOOR)&&valid_coordinate)
							if (floor == FLOOR)
								//如果是最外层，记录此时坐标
							{
								best_coordinate[0] = raw;
								best_coordinate[1] = column;
							}
							//这个剪枝待修改
							else
							{
								if (best_score > best_score_of_upper[floor])//剪枝
								{
									strncpy(board[raw][column], temp_blank, 2);
									return 89999900;
								}
							}
						}
						//复原
						strncpy(board[raw][column], temp_blank, 2);
						best_score_of_upper[floor - 1] = best_score;
					}


				}
			}

		}
		else
		{
			//best_score_of_upper[floor] = 0;
			before_evaluation(board, priority, floor, step_count, my_turn);
			for (int test_raw = 0; test_raw < 4; test_raw++)
			{
				for (int test_raw2 = 0; test_raw2 < 26; test_raw2++)
				{

					printf("%d,", priority[test_raw][test_raw2][0]);
					printf("%d", priority[test_raw][test_raw2][1]);
					printf(" ");
				}
				printf("\n");
			}

			for (int a = 0; a < 26; a++)
			{
				int raw = priority[FLOOR - floor][a][0];
				int column = priority[FLOOR - floor][a][1];
				if ((raw != 0) || (column != 0))
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						strncpy(temp_blank, board[raw][column], 2);
						strncpy(board[raw][column], chess, 2);
						DrawBoard(board, 15, temp_score, 2);
						temp_score = Minimax2(board, step_count + 1,
							!my_turn, ai_first,
							floor - 1, coordinate, best_score_of_upper, priority);
						if ((temp_score != 0) && (best_score == 0))
						{
							best_score = temp_score;

						}
						if (temp_score < best_score)
						{
							best_score = temp_score;
							//这里没有那个最外层判定坐标的东西，因为最外层是不可能会出现传递min的情况的

							if (floor == FLOOR)
								//如果是最外层，记录此时坐标
							{
								best_coordinate[0] = raw;
								best_coordinate[1] = column;
							}
							else
							{
								if (best_score < best_score_of_upper[floor])//剪枝
								{
									strncpy(board[raw][column], temp_blank, 2);
									return -89999900;
								}
							}
						}
						strncpy(board[raw][column], temp_blank, 2);
						if (best_score > best_score_of_upper[floor - 1])
						{
							best_score_of_upper[floor - 1] = best_score;
						}

					}
				}

			}

			//下面是废弃的原来的遍历整个棋盘的循环
			
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 1; column < 16; column++)
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						strncpy(temp_blank, board[raw][column], 2);
						strncpy(board[raw][column], chess, 2);
						//DrawBoard(board, 15, temp_score, 2);
						temp_score = Minimax2(board, step_count + 1,
							!my_turn, ai_first,
							floor - 1, coordinate, best_score_of_upper);
						if ((temp_score != 0) && (best_score == 0))
						{
							best_score = temp_score;

						}
						if (temp_score < best_score)
						{
							best_score = temp_score;
							//这里没有那个最外层判定坐标的东西，因为最外层是不可能会出现传递min的情况的

							if (floor == FLOOR)
								//如果是最外层，记录此时坐标
							{
								best_coordinate[0] = raw;
								best_coordinate[1] = column;
							}
							else
							{
								if (best_score < best_score_of_upper[floor])//剪枝
								{
									strncpy(board[raw][column], temp_blank, 2);
									return -89999900;
								}
							}
						}
						strncpy(board[raw][column], temp_blank, 2);
						best_score_of_upper[floor - 1] = best_score;
					}
				}
			}
		}

	}
	//最底层↓
	else
	{
		long int temp_score1 = 0;
		long int temp_score2 = 0;
		//找最佳的点与最值
		if (my_turn)
		{
			temp_score = 0;
			//best_score_of_upper[floor] = 0;
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 1; column < 16; column++)
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						//temp_score = evaluation(board, step_count, my_turn, raw, column);
						temp_score1 = evaluation(board, step_count, my_turn, raw, column);
						temp_score2 = evaluation(board, step_count + 1, !my_turn, raw, column);
						temp_score = temp_score1 + temp_score2;
						if (temp_score > best_score_of_upper[floor])//剪枝
						{

							return 89999900;
						}
						if (temp_score > best_score)
						{
							best_score = temp_score;
							//best_coordinate[0] = raw;
								//best_coordinate[1] = column;
						}
					}
				}
			}
		}
		else
		{
			temp_score = 0;
			//best_score_of_upper[floor] = 0;
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 1; column < 16; column++)
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						//temp_score = evaluation(board, step_count, my_turn, raw, column);
						temp_score1 = evaluation(board, step_count, my_turn, raw, column);
						temp_score2 = evaluation(board, step_count + 1, !my_turn, raw, column);
						temp_score = temp_score1 + temp_score2;
						if (temp_score < best_score_of_upper[floor])//剪枝
						{

							return -89999900;
						}
						if (temp_score < best_score)
						{
							best_score = temp_score;
							//best_coordinate[0] = raw;
								//best_coordinate[1] = column;
						}
					}

				}
			}
		}
	}

	//最外层，将要返回一个最终决定的最优坐标
	if ((FLOOR - floor) == 0)
	{
		*coordinate = *best_coordinate;
		*(coordinate + 1) = *(best_coordinate + 1);
		best_score = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
	}



	//strncpy(board[best_coordinate[0]][best_coordinate[1]], temp_blank, 2);


	return best_score;
}
*/


/*
if (mode_choice == 1)
{
	DrawBoard(board, 15, value, mode_choice, coordinate, -1);
	while (continue_playing)
	{
		//在这个循环里面试着将评分函数混进去
		//首先尝试着将自己下的每一步用评分函数打个分吧



		//chess_play(board, step_count);老的chessplay函数
		get_coordinate(coordinate, board, step_count);
		chess_play_ver2(board, step_count, coordinate);
		value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
		//my_value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
		//opponent_value = evaluation(board, step_count + 1, !my_turn, coordinate[0], coordinate[1]);
		//value = my_value + opponent_value;
		//上面这个分开打分的可能有问题
		DrawBoard(board, 15, value, mode_choice, coordinate, step_count);


		return_to_normal_chess(board, step_count, coordinate, coordinate);

		continue_playing = judgement(board, step_count);
		my_turn = !my_turn;
		step_count++;
	}
	DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
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
*/


/*
else
	{
		//PVE

		printf("电脑先手还是对方先手？\n");
		printf("1.电脑先手\t\t2.对方先手\n");
		printf("************************************************\n");
		int ai_choice = 0;
		int ai_choice_index = 0;
		int floor = FLOOR;//搜索层数
		char *chess;
		char *opponent_chess;
		bool invalid_ai_choice = true;
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
			chess = black;
			opponent_chess = white;
			strncpy(board[7][8], chess, 2);
		}
		else
		{
			my_turn = false;
			chess = white;
			opponent_chess = black;

		}
		while (continue_playing)
		{
			DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
			//注意这里无需将my_turn求反了，在αβ剪枝函数中已经做了
			int priority[4][26][2] =
			{
				{
					{0,0},{0,0},//连五
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//活四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//冲四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//连三
				},

				{
					{0,0},{0,0},//连五
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//活四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//冲四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//连三
				},

				{
					{0,0},{0,0},//连五
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//活四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//冲四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//连三
				},

				{
					{0,0},{0,0},//连五
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//活四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//冲四
					{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},//连三
				}
			};//连五双方只需要1个，连四、双活三、冲四、活三均给三组坐标（暂定）
			//第一层数组表示的是，minimax函数搜索的层数，每一层用一个本维度的数组
			//第二层数组从左到右依次对应的坐标是：
			//我方连五，对方连五，我方（双）活四，对方（双）活四（这个规则可能需要进一步细分）
			//我方双活三，对方双活三，我方冲四，对方冲四，我方活三，对方活三
			//第三层就是横纵坐标了
			long int best_score_of_upper[FLOOR] = { -89999900 , 89999900 , -89999900 };//给minimax里面的剪枝用的
			bool not_in_the_same_branch[FLOOR] = { true, true, true };
			if (my_turn)
			{

				if (step_count > 4)
				{
					value = Minimax2(board, step_count, my_turn, ai_first, floor, coordinate, best_score_of_upper, priority, not_in_the_same_branch);
					if ((coordinate[0] == 0) && (coordinate[1] == 1))
					{
						auto_play(board, chess, opponent_chess);
					}
					else
					{
						chess_play_ver2(board, step_count, coordinate);
					}

				}
				else
				{
					auto_play(board, chess, opponent_chess);
					/*for (int p = 0; p < 15; p++)
					{
						for (int q = 0; q < 17; q++)
						{
							if ((strncmp(board[p][q], opponent_chess, 2)) == 0)
							{
								if (((strncmp(board[p + 1][q], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p + 1][q], chess, 2)) != 0))
								{
									strncpy(board[p + 1][q], chess, 2);

								}
								else if (((strncmp(board[p][q + 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p][q + 1], chess, 2)) != 0))
								{
									strncpy(board[p][q + 1], chess, 2);
								}
								else if (((strncmp(board[p - 1][q], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p - 1][q], chess, 2)) != 0))
								{
									strncpy(board[p - 1][q], chess, 2);
								}
								else if (((strncmp(board[p][q - 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p][q - 1], chess, 2)) != 0))
								{
									strncpy(board[p][q - 1], chess, 2);
								}
								else if (((strncmp(board[p - 1][q - 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p - 1][q - 1], chess, 2)) != 0))
								{
									strncpy(board[p - 1][q - 1], chess, 2);
								}
								else if (((strncmp(board[p - 1][q + 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p - 1][q + 1], chess, 2)) != 0))
								{
									strncpy(board[p - 1][q + 1], chess, 2);
								}
								else if (((strncmp(board[p + 1][q + 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p + 1][q + 1], chess, 2)) != 0))
								{
									strncpy(board[p + 1][q + 1], chess, 2);
								}
								else if (((strncmp(board[p + 1][q - 1], opponent_chess, 2)) != 0)
									&& ((strncmp(board[p + 1][q - 1], chess, 2)) != 0))
								{
									strncpy(board[p + 1][q - 1], chess, 2);
								}
							}
						}
					}
				}
			}
			else
			{
			get_coordinate(coordinate, board, step_count);
			chess_play_ver2(board, step_count, coordinate);
			value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
			}

			continue_playing = judgement(board, step_count);
			my_turn = !my_turn;
			step_count++;
		}
		DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
		if (step_count % 2)
		{
			printf("黑子获胜");
		}
		else
		{
			printf("白子获胜");
		}

		return;


		printf("施工中\n");

*/



/*
	else
	{
	long int temp_score1 = 0;
	long int temp_score2 = 0;
	int best_raw = 0;
	int best_column = 1;
	//找最佳的点与最值
	if (my_turn)
	{
		temp_score = 0;
		//best_score_of_upper[floor] = 0;
		//先搜索哈希表，如果有就不打分了
		temp_score1 = Zobrist_hashing(hashing_value, key, best_raw, best_column, false, step_count, board, my_turn, hashing_value_now);
		temp_score2 = Zobrist_hashing(hashing_value, key, best_raw, best_column, false, step_count + 1, board, !my_turn, hashing_value_now);
		if (temp_score1 == 0 && temp_score2 == 0)
		{//如果哈希表没有值
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 1; column < 16; column++)
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						//temp_score = evaluation(board, step_count, my_turn, raw, column);

						temp_score1 = evaluation(board, step_count, my_turn, raw, column);
						temp_score2 = evaluation(board, step_count + 1, !my_turn, raw, column);
						temp_score = temp_score1 + temp_score2;
						if (temp_score != 0)
						{


							if (best_score == 0)
							{
								best_score = temp_score;
								best_raw = raw;
								best_column = column;

							}
							if (temp_score > best_score)
							{
								best_score = temp_score;
								best_raw = raw;
								best_column = column;
								//best_coordinate[0] = raw;
									//best_coordinate[1] = column;
							}
							/*
							if ((best_score < best_score_of_upper[floor]) && not_in_the_same_branch[floor])//剪枝
							{

								return -89999900;
							}
							*//*
						}
					}
				}
			}


			//在循环结束后，将最佳的坐标输入进哈希表里面
			Zobrist_hashing(hashing_value, key, best_raw, best_column, true, step_count, board, my_turn, hashing_value_now);



		}*/
		/*
		for (int raw = 0; raw < 15; raw++)
		{
			for (int column = 1; column < 16; column++)
			{
				if ((strncmp(board[raw][column], chess, 2) != 0)
					&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
				{
					//temp_score = evaluation(board, step_count, my_turn, raw, column);

					temp_score1 = evaluation(board, step_count, my_turn, raw, column);
					temp_score2 = evaluation(board, step_count + 1, !my_turn, raw, column);
					temp_score = temp_score1 + temp_score2;
					if (temp_score != 0)
					{


						if (best_score == 0)
						{
							best_score = temp_score;
							best_raw = raw;
							best_column = column;

						}
						if (temp_score > best_score)
						{
							best_score = temp_score;
							best_raw = raw;
							best_column = column;
							//best_coordinate[0] = raw;
								//best_coordinate[1] = column;
						}
						/*
						if ((best_score < best_score_of_upper[floor]) && not_in_the_same_branch[floor])//剪枝
						{

							return -89999900;
						}
						*/

		/*
		else
		{
			best_score = temp_score1 + temp_score2;
		}






	}
	else
	{
		temp_score = 0;
		temp_score1 = Zobrist_hashing(hashing_value, key, best_raw, best_column, false, step_count, board, my_turn, hashing_value_now);
		temp_score2 = Zobrist_hashing(hashing_value, key, best_raw, best_column, false, step_count + 1, board, !my_turn, hashing_value_now);
		if (temp_score1 == 0 && temp_score2 == 0)
		{


			//best_score_of_upper[floor] = 0;
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 1; column < 16; column++)
				{
					if ((strncmp(board[raw][column], chess, 2) != 0)
						&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
					{
						//temp_score = evaluation(board, step_count, my_turn, raw, column);
						temp_score1 = evaluation(board, step_count, my_turn, raw, column);
						temp_score2 = evaluation(board, step_count + 1, !my_turn, raw, column);
						temp_score = temp_score1 + temp_score2;
						if (temp_score != 0)
						{


							if (best_score == 0)
							{
								best_score = temp_score;
								best_raw = raw;
								best_column = column;
							}

							if (temp_score < best_score)
							{
								best_score = temp_score;
								best_raw = raw;
								best_column = column;
								//best_coordinate[0] = raw;
									//best_coordinate[1] = column;
							}
							/*
							if ((best_score > best_score_of_upper[floor]) && not_in_the_same_branch[floor])//剪枝
							{

								return 89999900;
							}
							*/
		/*
						}
					}

				}
			}

			//在循环结束后，将最佳的坐标输入进哈希表里面
			Zobrist_hashing(hashing_value, key, best_raw, best_column, true, step_count, board, my_turn, hashing_value_now);



		}
		else
		{
			best_score = temp_score1 + temp_score2;
		}

	}
	}
	*/