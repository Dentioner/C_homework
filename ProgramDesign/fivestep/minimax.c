#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"head.h"
#include<time.h>


long int Minimax2(char board[][17][3], int step_count,
	bool my_turn, bool ai_first,
	int floor, int coordinate[], long int best_score_of_upper[], int priority[][26][2], bool not_in_the_same_branch[], 
	unsigned long long hashValue, unsigned long long ZobristTable[15][15][2], unsigned long long hashing_value2[depth_of_hashing][3],
	int fatal_priority[][32][2], long int fatal_best_score_of_upper[], bool fatal_not_in_the_same_branch[])
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
	
	if (floor != 0)
	{
		if (my_turn)
		{
			//best_score_of_upper[floor] = 0;
			//先将优先的那些点找到并递归
			final_hit = before_evaluation(board, priority, floor, step_count, my_turn);
			
			
			
			//下面这个双层的for循环是在测试的时候输出的，正式使用的时候可以关掉
			/*
			for (int test_raw = 0; test_raw < 10; test_raw++)
			{
				for (int test_raw2 = 0; test_raw2 < 26; test_raw2++)
				{
					
					printf("%d,", priority[test_raw][test_raw2][0]);
					printf("%d", priority[test_raw][test_raw2][1]);
					printf(" ");
				}
				printf("\n");
			}
			*/



			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//final_hit 下面这一块可能需要重新写一下，仿照井字棋的写法
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





			if (final_hit
				&&(FLOOR == floor))
			{//如果发现有连五，直接秒了
				coordinate[0] = priority[0][0][0];
				coordinate[1] = priority[0][0][1];
				best_score = evaluation(board, step_count, my_turn, priority[0][0][0], priority[0][0][1]);
				return best_score;
			}
			//下面这个if语句是新加的迭代加深
			if (final_hit && (FLOOR != floor))
			{
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
			//下面是尝试加上算杀，目前只想到在最外层开搜，在别的层数搜索暂时无法实现
/*
			if (priority[FLOOR - floor][8][0] != 0 || priority[FLOOR - floor][8][1] != 0 
				|| priority[FLOOR - floor][11][0] != 0 || priority[FLOOR - floor][11][1] != 0)//如果找到了双活三的点的话，就进行算杀
			{
				if (floor == FLOOR)
				{
					int floor_vcx = FLOOR_VCX;
					int fatal_best_coordinate[2] = { 0,1 };
				
					best_score = fatal_step(board, step_count, my_turn, ai_first, floor_vcx, fatal_best_coordinate, fatal_best_score_of_upper, fatal_priority, fatal_not_in_the_same_branch);
					printf("%d,%d\n", priority[FLOOR - floor][8][0], priority[FLOOR - floor][8][1]);
					printf("%d,%d\n", priority[FLOOR - floor][11][0], priority[FLOOR - floor][11][1]);
					system("pause");
				}
				
			}
			*/

			//else
			//{

			bool initialized = false;//false表示best_score还没有被赋值过
				for (int a = 0; a < 26; a++)
				{
					
					not_in_the_same_branch[floor - 1] = true;//判断是否在同一分支中，以免误剪枝
					int raw = priority[FLOOR - floor][a][0];
					int column = priority[FLOOR - floor][a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((strncmp(board[raw][column], chess, 2) != 0)
							&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
						{
							//初始化剪枝的参数
							if (floor - 2 >= 0)
							{
								best_score_of_upper[floor - 2] = 89999900;
								//not_in_the_same_branch[floor - 2] = true;
							}
							strncpy(temp_blank, board[raw][column], 2);
							strncpy(board[raw][column], chess, 2);
							hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
							//下面这行是在测试的时候使用的，正式使用的时候关掉
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);
							long int temp_score1 = Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
							long int temp_score2 = Searching_Hashing2(hashing_value2, ZobristTable, step_count + 1, hashValue, !my_turn, 0, false);
							if (temp_score1 == 0 && temp_score2 == 0)
							{
								temp_score = Minimax2(board, step_count + 1,
									!my_turn, ai_first,
									floor - 1, coordinate, best_score_of_upper, priority, not_in_the_same_branch, hashValue, ZobristTable, hashing_value2, fatal_priority, fatal_best_score_of_upper, fatal_not_in_the_same_branch);
							}
							else
							{
								temp_score = temp_score1 + temp_score2;
							}
							//下面这行是在测试的时候使用的，正式使用的时候关掉
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);


							//下面是从井字棋那里搬过来的


							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;

								if (floor == FLOOR)
									//如果是最外层，记录此时坐标
								{
									best_coordinate[0] = raw;
									best_coordinate[1] = column;
								}
								//这个剪枝待修改
								else
								{
									if ((best_score >= best_score_of_upper[floor]) && (not_in_the_same_branch[floor]))//剪枝
									{
										strncpy(board[raw][column], temp_blank, 2);
										return 89999900;
									}

								}

							}

							else
							{

							
								
								if (temp_score >= best_score)
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
										if ((best_score >= best_score_of_upper[floor])&&(not_in_the_same_branch[floor]))//剪枝
										{
											strncpy(board[raw][column], temp_blank, 2);
											return 89999900;
										}
								
									}
								}
							//复原
							}
							strncpy(board[raw][column], temp_blank, 2);
							Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, temp_score, true);
							hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
							if (best_score > best_score_of_upper[floor - 1])
							{
								best_score_of_upper[floor - 1] = best_score;
								not_in_the_same_branch[floor - 1] = false;
							}
							if (best_score == 89999900)
							{
								return best_score;
							}
						
						}
					}

				


				}
		
			//}
		}
		else
		{
			//best_score_of_upper[floor] = 0;
			final_hit = before_evaluation(board, priority, floor, step_count, my_turn);
			

			
			//下面这个双层的for循环是在测试的时候输出的，正式使用的时候可以关掉
			/*
			for (int test_raw = 0; test_raw < 10; test_raw++)
			{
				for (int test_raw2 = 0; test_raw2 < 26; test_raw2++)
				{
					
					printf("%d,", priority[test_raw][test_raw2][0]);
					printf("%d", priority[test_raw][test_raw2][1]);
					printf(" ");
				}
				printf("\n");
			}
			*/

			//下面这个if是新加的迭代加深
			if (!final_hit)
			{
				if ((priority[FLOOR - floor][1][0] != 0) || (priority[FLOOR- floor][1][1]) != 0)
				{
					
					best_score = evaluation(board, step_count, my_turn, priority[FLOOR - floor][1][0], priority[FLOOR - floor][1][1]);
					return best_score;
				}

			}
			//下面这个else if 的实用性待验证，先试着用一下
			
			else if (final_hit)
			{
				if ((priority[FLOOR - floor][1][0] == 0) && ((priority[FLOOR - floor][1][1]) == 0))
				{
					best_score = evaluation(board, step_count, my_turn, priority[FLOOR - floor][0][0], priority[FLOOR - floor][0][1]);
					return best_score;
				}
			}
			bool initialized = false;//false表示best_score还没有被赋值过
				for (int a = 0; a < 26; a++)
				{
					not_in_the_same_branch[floor - 1] = true;
					int raw = priority[FLOOR - floor][a][0];
					int column = priority[FLOOR - floor][a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((strncmp(board[raw][column], chess, 2) != 0)
							&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
						{
							//初始化剪枝的参数
							if (floor - 2 >= 0)
							{
								best_score_of_upper[floor - 2] = -89999900;
								//not_in_the_same_branch[floor - 2] = true;
							}
							strncpy(temp_blank, board[raw][column], 2);
							strncpy(board[raw][column], chess, 2);
							hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
							long int temp_score1 = Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
							long int temp_score2 = Searching_Hashing2(hashing_value2, ZobristTable, step_count + 1, hashValue, !my_turn, 0, false);
						
							//下面这个是在测试的时候输出的，正式使用的时候可以关掉
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);
							if (temp_score1 == 0 && temp_score2 == 0)
							{
								temp_score = Minimax2(board, step_count + 1,
									!my_turn, ai_first,
									floor - 1, coordinate, best_score_of_upper, priority, not_in_the_same_branch, hashValue, ZobristTable, hashing_value2, fatal_priority, fatal_best_score_of_upper, fatal_not_in_the_same_branch);
							}
							else
							{
								temp_score = temp_score1 + temp_score2;
							}


							//下面是从井字棋那里搬过来的
							//if ((temp_score != 0) && (best_score == 0))
		//之所以把这里的temp_score != 0也给去掉，是因为如果最后一个枝是一个大于0的数，那么它会直接赋给bestscore
		//但是这一层是越小越好，如果之前有0的枝，但却被替代了，是不合理的
		//例如先搜索到一个打分为0的点，最后一次搜到了打分为100的点，虽然应该向上一层传递0分的，但是由于这里的逻辑判断，传上去了100分
		//但是什么都不写也不行，因为如果有负分，在“传递最小值”的情况下则会无法成为最优值
									//这里应该采用状态机的方法，在搜索第一个值的时候保证会赋给bests
							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;

								if (floor == FLOOR)
									//如果是最外层，记录此时坐标
								{
									best_coordinate[0] = raw;
									best_coordinate[1] = column;
								}
								else
								{
									if ((best_score <= best_score_of_upper[floor]) && not_in_the_same_branch[floor])//剪枝
									{
										strncpy(board[raw][column], temp_blank, 2);
										return -89999900;
									}
								}

							}


							else
							{

							
								if (temp_score <= best_score)
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
										if ((best_score <= best_score_of_upper[floor])&&not_in_the_same_branch[floor])//剪枝
										{
											strncpy(board[raw][column], temp_blank, 2);
											return -89999900;
										}
									}
								}
							}

							strncpy(board[raw][column], temp_blank, 2);
							Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, temp_score, true);
							hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
							if (best_score > best_score_of_upper[floor - 1])
							{
								best_score_of_upper[floor - 1] = best_score;
								not_in_the_same_branch[floor - 1] = false;
							}
							if (best_score == -89999900)
							{
								return best_score;
							}
						
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
		int best_raw = 0;
		int best_column = 1;
		//int floor_vcx = FLOOR_VCX;//这个是算杀专用floor
		//int fatal_best_coordinate[2] = { 0,1 };
		//找最佳的点与最值
		if (my_turn)
		{
			temp_score = 0;
			//best_score_of_upper[floor] = 0;
			//先搜索哈希表，如果有就不打分了
			//temp_score1 = Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
			//temp_score2 = Searching_Hashing2(hashing_value2, ZobristTable, step_count + 1, hashValue, !my_turn, 0, false);
			
			
			
			temp_score = Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
			
			//if (temp_score1 == 0 && temp_score2 == 0)
			if (temp_score == 0)
			
			{//如果哈希表没有值
				//下面是在最底层进行算杀的试用版，目前废弃
				/*
				temp_score = fatal_step(board, step_count, my_turn, ai_first, floor_vcx, fatal_best_coordinate, fatal_best_score_of_upper, fatal_priority, fatal_not_in_the_same_branch);
				if (fatal_best_coordinate[0] == 0 && fatal_best_coordinate[1] == 0)
				{*/

				
				
					//这个for循环是一开始就有的，别把这个给删了
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
								
								temp_score1 = abs(temp_score1) * 1.5;
								temp_score2 = abs(temp_score2) * 0.75;
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
								}
							}
						}
					}
				//下面这个大括号是和上面的那个算杀if配套的
				//}
				//下面这个也是废弃的算杀
				/*
				else
				{
					best_score = evaluation(board, step_count, my_turn, fatal_best_coordinate[0], fatal_best_coordinate[1]);
				}*/

				//在循环结束后，将最佳的坐标输入进哈希表里面
				Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, temp_score, true);
				
			
			
			}
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
				

			else
			{
				//temp_score1 = abs(temp_score1) * 1.5;
				//temp_score2 = abs(temp_score2) * 0.75;
				//temp_score = temp_score1 + temp_score2;
				best_score = temp_score;
			}
				
			
			
			
			
		
		}
		else
		{
			temp_score = 0;
			//temp_score1 = Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
			//temp_score2 = Searching_Hashing2(hashing_value2, ZobristTable, step_count + 1, hashValue, !my_turn, 0, false);
			temp_score = Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);


			//if (temp_score1 == 0 && temp_score2 == 0)
			if (temp_score == 0)
			
			{
				//下面先在最底层进行算杀，试用版
				/*
				temp_score = fatal_step(board, step_count, my_turn, ai_first, floor_vcx, fatal_best_coordinate, fatal_best_score_of_upper, fatal_priority, fatal_not_in_the_same_branch);
				if (fatal_best_coordinate[0] == 0 && fatal_best_coordinate[1] == 0)
					//best_score_of_upper[floor] = 0;
				{*/

					//下面这个for循环别删了
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
								}
							}
			
						}
					}
				//下面这个也是算杀的
				/*}
				
				else
				{
					best_score = evaluation(board, step_count, my_turn, fatal_best_coordinate[0], fatal_best_coordinate[1]);
				}
				*/


				//在循环结束后，将最佳的坐标输入进哈希表里面
				Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, temp_score, true);

			
			
			}
			else
			{
				//best_score = temp_score1 + temp_score2;
				best_score = temp_score;
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
