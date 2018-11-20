#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include"head.h"
#include<time.h>

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


	char board[15][17][3] =
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
	char black[3] = "○";
	char white[3] = "●";
	char roaming[3] = "△";//随便定义的一个，这个是用来记录上一步的空格在哪个位置，便于悔棋

	//下面几个变量是zobrist哈希算法使用的
	long long int key[15][15][2] = { 0 };//第三维度分别代表黑子和白子，0为空白
	long long int hashing_value[depth_of_hashing][3] = { {0,0,0} };
	//第二维度的[0]是整个棋盘的哈希值，[1]与[2]是此哈希值对应的棋盘评分，应该定义在函数外面
	long long int hashing_value_now = 0;//目前的哈希值，这个是要在minimax.c里面进行运算的
	//以上是旧的哈希相关的变量

	unsigned long long ZobristTable[15][15][2];//梅森旋转的哈希键值表，对应于旧算法的key数组
	unsigned long long hashValue = 0;//梅森旋转算法下，棋盘的哈希值，对应于旧算法的hashing_value_now
	unsigned long long hashing_value2[depth_of_hashing][3] = { {0,0,0} };//第二维度的[0]是整个棋盘的哈希值
	//[1]与[2]是此哈希值对应的棋盘评分，应该定义在函数外面
	//上面这个是新的棋盘哈希表，注意评分在输出的时候要进行类型转换

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
		initTable(ZobristTable);//初始化哈希表（键值表）
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
		DrawBoard(board, 15, value, mode_choice, coordinate, -1);
		while (continue_playing)
		{
				
			
			//chess_play(board, step_count);老的chessplay函数
			get_coordinate(coordinate, board, step_count);
			strncpy(roaming, board[coordinate[0]][coordinate[1]], 2);//记录上一步的状态
			chess_play_ver2(board, step_count, coordinate);
			//下面在PVP里面试用哈希函数，由于PVP不会搜索，因此思路是，先悔棋，然后再看看有没有将哈希值存进表里面
			//hashValue = computeHash(board, ZobristTable);//计算当前棋局的哈希值
			//上面那个hashValue先挂起，可能需要优化，先用下面这个语句代替

			//在将coordinate使用之前，先将它与zobrist表统一一下，因为coordinate不可能有（0,0），但是zobrist却有
			//这就是为什么coordinate[1]要减1的原因
			hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];
			
			value = Searching_Hashing(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
			if (value == 0)
			{
				value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
				Searching_Hashing(hashing_value2, ZobristTable, step_count, hashValue, my_turn, value, true);
			}
			

			
			//my_value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
			//opponent_value = evaluation(board, step_count + 1, !my_turn, coordinate[0], coordinate[1]);
			//value = my_value + opponent_value;
			//上面这个分开打分的可能有问题
			DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
			return_to_normal_chess(board, step_count, coordinate, coordinate);
			


			printf("是否想要悔棋？按y悔棋，按别的任意键正常继续游戏.\n");
			char c_getback = ' ';
			c_getback = getchar();
			while (getchar() != '\n')
				continue;
			int i_getback = c_getback;
			if (i_getback == 89 || i_getback == 121)
			{
				strncpy(board[coordinate[0]][coordinate[1]], roaming, 2);
				DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
				hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];//再一次异或回来
				continue;
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
	}


	else
	{
		//PVE
		initTable(ZobristTable);
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
			my_turn = false;
			step_count++;
		}
		else
		{
			my_turn = false;
			chess = white;
			opponent_chess = black;

		}
		DrawBoard(board, 15, value, mode_choice, coordinate, -1);
		//initialize_hashing_sheet(key);
		while (continue_playing)
		{
			
			//注意这里无需将my_turn求反了，在αβ剪枝函数中已经做了
			int priority[10][26][2] =
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
			long int best_score_of_upper[11] = { -89999900 , 89999900 , -89999900, 89999900 , -89999900, 89999900, - 89999900 , 89999900 , -89999900, 89999900, -89999900 };//给minimax里面的剪枝用的
			//注意上下这两个数组的编号问题。floor是从11往0递归的，因此要保持最后一个元素不变。
			bool not_in_the_same_branch[11] = { true, true, true, true, true, true, true, true, true, true, true };
			//下面3个带fatal_字头的是杀棋专用的数组
			int fatal_priority[6][32][2] =
			{
				{
					{0,0},//自己的连五
					{0,0},//对方的连五
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的冲四
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的冲四
				},
				//下同
				{
					{0,0},//自己的连五
					{0,0},//对方的连五
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
				},

				{
					{0,0},//自己的连五
					{0,0},//对方的连五
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
				},

				{
					{0,0},//自己的连五
					{0,0},//对方的连五
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
				},
				{
					{0,0},//自己的连五
					{0,0},//对方的连五
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
				},
				{
					{0,0},//自己的连五
					{0,0},//对方的连五
					{0,0},{0,0},{0,0},{0,0},{0,0},//自己的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},//对面的双活三
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
					{0,0},{0,0},{0,0},{0,0},{0,0},
				}
			};
			long int fatal_best_score_of_upper[7] = { -89999900 , 89999900 , -89999900, 89999900 , -89999900, 89999900, -89999900 };
			bool fatal_not_in_the_same_branch[7] = { true, true, true, true, true, true, true };
			if (my_turn)
			{
				double start_time = clock();
				double end_time, cost_time;
				if (step_count > 2)
				{
					value = Minimax2(board, step_count, my_turn, ai_first, floor, coordinate, best_score_of_upper, priority, not_in_the_same_branch, hashValue, ZobristTable, hashing_value2, fatal_priority, fatal_best_score_of_upper, fatal_not_in_the_same_branch);
					if ((coordinate[0] == 0) && (coordinate[1] == 1))
					{
						auto_play(board, chess, opponent_chess, coordinate);
						chess_play_ver2(board, step_count, coordinate);
						hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];
						DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
						return_to_normal_chess(board, step_count, coordinate, coordinate);
					}
					else
					{
						chess_play_ver2(board, step_count, coordinate);
						hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];
						DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
						return_to_normal_chess(board, step_count, coordinate, coordinate);
					}

					//测试，打印哈希表，看看表是否正常
					for (int ha1 = 0;ha1 < depth_of_hashing; ha1++)
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
				else
				{
					auto_play(board, chess, opponent_chess, coordinate);
					chess_play_ver2(board, step_count, coordinate);
					hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];
					DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
					return_to_normal_chess(board, step_count, coordinate, coordinate);
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
					}*/
				}
				end_time = clock();
				cost_time = (end_time - start_time) / CLK_TCK;
				printf("time=%fs.\n", cost_time);
				//printf("暂停中，按enter键继续");
				//system("pause");
			}
			else
			{
				get_coordinate(coordinate, board, step_count);
				//把value和chessplay换过位置了，不知道会怎样
				strncpy(roaming, board[coordinate[0]][coordinate[1]], 2);//记录上一步的状态
				value = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
				chess_play_ver2(board, step_count, coordinate);
				hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];
				DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
				return_to_normal_chess(board, step_count, coordinate, coordinate);
				//下面是悔棋代码，可能会有问题			
				printf("是否想要悔棋？按y悔棋，按别的任意键正常继续游戏.\n");
				char c_getback = ' ';
				c_getback = getchar();
				while (getchar() != '\n')
					continue;
				int i_getback = c_getback;
				if (i_getback == 89 || i_getback == 121)
				{
					strncpy(board[coordinate[0]][coordinate[1]], roaming, 2);
					hashValue ^= ZobristTable[coordinate[0]][coordinate[1] - 1][(step_count % 2)];
					DrawBoard(board, 15, value, mode_choice, coordinate, step_count);
					continue;
				}
			
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




	}

}

void DrawBoard(char board[][17][3], int i, long int value, int mode_choice, int coordinate[], int step_count)
{
	char c_column;
	int i_raw;
	i_raw = 15 - coordinate[0];
	c_column = 64 + coordinate[1];
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
	if (step_count != -1)
	{ 
		printf("最后落子(%d, %c), value = %ld\n", i_raw, c_column, value);
	}
	
	
}
