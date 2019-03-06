#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"head.h"
#include<time.h>
#include<math.h>

//本工程去除了哈希
extern int board[15][15];
extern int coordinate[2];
extern int roaming;
extern int w;//白棋
extern int b;//黑棋
extern unsigned long long ZobristTable[15][15][2];//梅森旋转的哈希键值表
extern unsigned long long hashValue;//梅森旋转算法下，棋盘的哈希值
extern unsigned long long hashing_value2[depth_of_hashing][3];
extern long int best_score_of_upper[11];
extern bool not_in_the_same_branch[11];
extern long int value_for_board;//新加
extern long int best_score_of_upper_ver2[12];
bool show_me_the_array =  false;//测试用的布尔值
extern bool ai_first;

long int Minimax2(int step_count, bool my_turn, bool ai_first, int floor)
{

	int temp_blank;//用这个来还原棋盘，相当于悔棋一样的
	int chess;
	int opponent_chess;
	//bool final_hit = false;
	int status = 0;
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五
	//2表示当前棋局没有发现我方连五，但是发现对方连五
	int priority_ver2[10][2] = { {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} };
	long int best_score = 0;
	int best_coordinate[2] = { 0,0 };
	long int temp_score = 0;
	//bool valid_coordinate = false;
	//下面是在建立ai先手、回合数与“是否是我方回合”的关系

	//下面这个条件语句是用来打断点进行单步调试用的，正常工作的时候要注释掉
	if (coordinate[0] == 11 && coordinate[1] == 6 && floor == FLOOR)
	{
		printf("\n");
	}


	if ((step_count % 2) == 0)
	{
		chess = b;
		opponent_chess = w;
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
		chess = w;
		opponent_chess = b;
		if (ai_first)
		{
			my_turn = false;
		}
		else
		{
			my_turn = true;
		}

	}


	if (floor != 0)
	{
		if (my_turn)
		{
			//best_score_of_upper[floor] = 0;
			//先将优先的那些点找到并递归
			//旧的启发式搜索
			//status = before_evaluation_ver2(board, priority, floor, step_count, my_turn);
			status = before_evaluation_ver4(priority_ver2, floor, step_count, my_turn);

			if (status != 0)
			{


				long int temp_score1 = 0;
				long int temp_score2 = 0;
				int best_raw = 0;
				int best_column = 1;
				bool initialized = false;

				//找最佳的点与最值

				temp_score = 0;
				
				//这个for循环是一开始就有的，别把这个给删了
				for (int raw = 0; raw < 15; raw++)
				{
					for (int column = 0; column < 15; column++)
					{
						if ((board[raw][column] != chess)
							&& (board[raw][column] != opponent_chess))
						{
							//temp_score = evaluation(board, step_count, my_turn, raw, column);

							temp_score1 = evaluation_ver2(step_count, my_turn, raw, column);
							temp_score2 = evaluation_ver2(step_count + 1, !my_turn, raw, column);

							temp_score1 = abs(temp_score1) * 1.5;
							temp_score2 = abs(temp_score2) * 0.75;
							temp_score = temp_score1 + temp_score2;

							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;
								best_raw = raw;
								best_column = column;

							}
							else
							{
								if (temp_score > best_score)
									//之所以不取等，是因为如果所有分支的分值都为0的话，就拿最开始出现的那个落点来下，因为最开始的落点是本层评分最高的
								{
									best_score = temp_score;
									if (floor == FLOOR)
										//如果是最外层，记录此时坐标
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
										best_raw = raw;
										best_column = column;

									}
								}
							}
						}
					}
				}







			}
			else
			{


				bool initialized = false;//false表示best_score还没有被赋值过
				
				for (int a = 0; a < 10; a++)
				{
					not_in_the_same_branch[floor - 1] = true;//判断是否在同一分支中，以免误剪枝
					int raw = priority_ver2[a][0];
					int column = priority_ver2[a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((board[raw][column] != chess)
							&& (board[raw][column] != opponent_chess))
						{
							//初始化剪枝的参数
							if (floor - 2 >= 0)
							{
								best_score_of_upper[floor - 2] = infinity;
								
							}
							temp_blank = board[raw][column];
							board[raw][column] = chess;
							//测试用，打印ZobristTable，看看是否正常
							/*
							for (int z1 = 0; z1 < 15; z1++)
							{
								for (int z2 = 0; z2 < 15; z2++)
								{
									printf("%llu, %llu\n", ZobristTable[z1][z2][0], ZobristTable[z1][z2][1]);
								}
							}
							*/

							temp_score = 0;
							//下面这几行是在测试的时候使用的，正式使用的时候关掉

							//DrawBoard(board, 15, 0, 2, coordinate, step_count);


							
							if (temp_score == 0)
							{
								temp_score = Minimax2(step_count + 1, !my_turn, ai_first, floor - 1);
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
									if ((best_score > best_score_of_upper[floor]) && (not_in_the_same_branch[floor]))//剪枝
									{
										board[raw][column] = temp_blank;
										return infinity;
									}

								}

							}

							else
							{



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
										if ((best_score > best_score_of_upper[floor]) && (not_in_the_same_branch[floor]))//剪枝
										{
											board[raw][column] = temp_blank;
											return infinity;
										}

									}
								}
								//复原
							}
							board[raw][column] = temp_blank;
							
							if (best_score > best_score_of_upper[floor - 1])
							{
								best_score_of_upper[floor - 1] = best_score;
								not_in_the_same_branch[floor - 1] = false;
							}
							if (best_score == infinity)
							{
								return best_score;
							}

						}
					}




				}

			}
		}
		else
		{
			//best_score_of_upper[floor] = 0;
			status = before_evaluation_ver4(priority_ver2, floor, step_count, my_turn);
			//final_hit = before_evaluation(board, priority, floor, step_count, my_turn);


			if (status != 0)
			{
				long int temp_score1 = 0;
				long int temp_score2 = 0;
				int best_raw = 0;
				int best_column = 0;
				bool initialized = false;

				//找最佳的点与最值


				temp_score = 0;






				//下面这个for循环别删了
				for (int raw = 0; raw < 15; raw++)
				{
					for (int column = 0; column < 15; column++)
					{
						if ((board[raw][column] != chess)
							&& (board[raw][column] != opponent_chess))
						{
							//temp_score = evaluation(board, step_count, my_turn, raw, column);
							temp_score1 = evaluation_ver2(step_count, my_turn, raw, column);
							temp_score2 = evaluation_ver2( step_count + 1, !my_turn, raw, column);
							temp_score1 = abs(temp_score1) * 1.5;
							temp_score2 = abs(temp_score2) * 0.75;
							temp_score = -(temp_score1 + temp_score2);

							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;
								best_raw = raw;
								best_column = column;

							}
							else
							{
								if (temp_score < best_score)
								{
									best_score = temp_score;
									if (floor == FLOOR)
										//如果是最外层，记录此时坐标
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
										best_raw = raw;
										best_column = column;

									}
								}
							}
						}

					}
				}



			}
			else
			{

				
				bool initialized = false;//false表示best_score还没有被赋值过
				
				for (int a = 0; a < 10; a++)
				{

					not_in_the_same_branch[floor - 1] = true;
					int raw = priority_ver2[a][0];
					int column = priority_ver2[a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((board[raw][column] != chess)
							&& (board[raw][column] != opponent_chess))
						{
							//初始化剪枝的参数
							if (floor - 2 >= 0)
							{
								best_score_of_upper[floor - 2] = -infinity;
								//not_in_the_same_branch[floor - 2] = true;
							}
							temp_blank = board[raw][column];
							board[raw][column] = chess;
							temp_score = 0;
							//下面这个是在测试的时候输出的，正式使用的时候可以关掉
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);
							if (temp_score == 0)
							{
								temp_score = Minimax2(step_count + 1, !my_turn, ai_first, floor - 1);
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
									if ((best_score < best_score_of_upper[floor]) && not_in_the_same_branch[floor])//剪枝
									{
										board[raw][column] = temp_blank;
										return -infinity;
									}
								}

							}


							else
							{


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
										if ((best_score < best_score_of_upper[floor]) && not_in_the_same_branch[floor])//剪枝
										{
											board[raw][column] = temp_blank;
											return -infinity;
										}
									}
								}
							}

							board[raw][column] = temp_blank;
							
							if (best_score > best_score_of_upper[floor - 1])
							{
								best_score_of_upper[floor - 1] = best_score;
								not_in_the_same_branch[floor - 1] = false;
							}
							if (best_score == -infinity)
							{
								return best_score;
							}

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
		bool initialized = false;

		//找最佳的点与最值
		if (my_turn)
		{
			temp_score = 0;
			//这里删了一堆注释，要恢复的去看别的地方存档的minimax文件


					//这个for循环是一开始就有的，别把这个给删了
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 0; column < 15; column++)
				{
					if ((board[raw][column] != chess)
						&& (board[raw][column] != opponent_chess))
					{
						//temp_score = evaluation(board, step_count, my_turn, raw, column);

						temp_score1 = evaluation_ver2(step_count, my_turn, raw, column);
						temp_score2 = evaluation_ver2(step_count + 1, !my_turn, raw, column);

						temp_score1 = abs(temp_score1) * 1.5;
						temp_score2 = abs(temp_score2) * 0.75;
						temp_score = temp_score1 + temp_score2;
						if (!initialized)
						{
							best_score = temp_score;
							initialized = true;
							best_raw = raw;
							best_column = column;

						}
						else
						{
							if (temp_score > best_score)
							{
								best_score = temp_score;

							}
						}
						
					}
				}
			}

		}
		else
		{
			temp_score = 0;


			//下面这个for循环别删了
			for (int raw = 0; raw < 15; raw++)
			{
				for (int column = 0; column < 15; column++)
				{
					if ((board[raw][column] != chess)
						&& (board[raw][column] != opponent_chess))
					{
						temp_score1 = evaluation_ver2(step_count, my_turn, raw, column);
						temp_score2 = evaluation_ver2(step_count + 1, !my_turn, raw, column);
						temp_score1 = abs(temp_score1) * 1.5;
						temp_score2 = abs(temp_score2) * 0.75;
						temp_score = -(temp_score1 + temp_score2);

						if (!initialized)
						{
							best_score = temp_score;
							initialized = true;
							best_raw = raw;
							best_column = column;

						}
						else
						{
							if (temp_score < best_score)
							{
								best_score = temp_score;

							}
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
		best_score = evaluation_ver2(step_count, my_turn, coordinate[0], coordinate[1]);
	}



	//strncpy(board[best_coordinate[0]][best_coordinate[1]], temp_blank, 2);


	return best_score;
}

long int Minimax3(int step_count, bool my_turn, int floor)
{

	int temp_blank;//用这个来还原棋盘，相当于悔棋一样的
	int chess;
	int opponent_chess;
	//bool final_hit = false;
	int status = 0; 
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五
	//2表示当前棋局没有发现我方连五，但是发现对方连五
	int priority_ver2[10][2] = { {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} };
	long int best_score = 0;
	int best_coordinate[2] = { 0,0 };
	long int temp_score = 0;
	//bool valid_coordinate = false;
	//下面是在建立ai先手、回合数与“是否是我方回合”的关系

	//下面这个条件语句是用来打断点进行单步调试用的，正常工作的时候要注释掉
	if (coordinate[0] == 0 && coordinate[1] == 0 && floor == FLOOR2)
	{
		printf("\n");
		show_me_the_array = true;
	}


	if ((step_count % 2) == 0)
	{
		chess = b;
		opponent_chess = w;
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
		chess = w;
		opponent_chess = b;
		if (ai_first)
		{
			my_turn = false;
		}
		else
		{
			my_turn = true;
		}

	}


	if (floor != 0)
	{
		if (my_turn)
		{
			//best_score_of_upper[floor] = 0;
			//先将优先的那些点找到并递归
			//旧的启发式搜索
			//status = before_evaluation_ver2(board, priority, floor, step_count, my_turn);
			status = before_evaluation_ver4(priority_ver2, floor, step_count, my_turn);

			if (status != 0)
			{
				if (floor == FLOOR2)//这种情况是，如果刚开始搜就发现有连五点，那就只搜这一层就退出
				{
					shallowest(step_count, my_turn);
					best_score = evaluation_ver2(step_count, my_turn, coordinate[0], coordinate[1]);
					return best_score;
				}
				else//这种情况是，在某一层（不是最外层）搜到了连五点，那就当做最底层开始搜
				{
					//best_score = deepest(step_count, my_turn);
					return infinity;
				}
			}
			else//这种情况是，一般的情况
			{


				bool initialized = false;//false表示best_score还没有被赋值过

				for (int a = 0; a < 10; a++)
				{
					not_in_the_same_branch[floor - 1] = true;//判断是否在同一分支中，以免误剪枝
					int raw = priority_ver2[a][0];
					int column = priority_ver2[a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((board[raw][column] != chess)
							&& (board[raw][column] != opponent_chess))
						{
							//初始化剪枝的参数
							if (floor - 2 >= 0)
							{
								best_score_of_upper_ver2[floor - 2] = infinity; 

							}
							temp_blank = board[raw][column];
							board[raw][column] = chess;
							//测试用，打印ZobristTable，看看是否正常
							/*
							for (int z1 = 0; z1 < 15; z1++)
							{
								for (int z2 = 0; z2 < 15; z2++)
								{
									printf("%llu, %llu\n", ZobristTable[z1][z2][0], ZobristTable[z1][z2][1]);
								}
							}
							*/

							temp_score = 0;
							//下面这几行是在测试的时候使用的，正式使用的时候关掉

							//DrawBoard(board, 15, 0, 2, coordinate, step_count);



							if (temp_score == 0)
							{
								temp_score = Minimax3(step_count + 1, !my_turn, floor - 1);
							}

							//下面这行是在测试的时候使用的，正式使用的时候关掉
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);


							//下面是从井字棋那里搬过来的


							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;

								if (floor == FLOOR2)
									//如果是最外层，记录此时坐标
								{
									best_coordinate[0] = raw;
									best_coordinate[1] = column;
								}
								//这个剪枝待修改

								else
								{
									if ((best_score > best_score_of_upper_ver2[floor]) && (not_in_the_same_branch[floor]))//剪枝
									{
										board[raw][column] = temp_blank;
										return infinity;
									}

								}

							}

							else
							{



								if (temp_score > best_score)
								{
									best_score = temp_score;

									//valid_coordinate = verify_coordinate(board, raw, column, chess, opponent_chess);
									//if ((floor == FLOOR)&&valid_coordinate)
									if (floor == FLOOR2)
										//如果是最外层，记录此时坐标
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
									}
									//这个剪枝待修改

									else
									{
										if ((best_score > best_score_of_upper_ver2[floor]) && (not_in_the_same_branch[floor]))//剪枝
										{
											board[raw][column] = temp_blank;
											return infinity;
										}

									}
								}
								//复原
							}
							board[raw][column] = temp_blank;

							if (best_score > best_score_of_upper_ver2[floor - 1])
							{
								best_score_of_upper_ver2[floor - 1] = best_score;
								not_in_the_same_branch[floor - 1] = false;
								if (best_score == 9375310)//测试用
									printf("\n");
							}
							if (best_score == infinity)
							{
								if (floor == FLOOR2)
								{
									coordinate[0] = best_coordinate[0];
									coordinate[1] = best_coordinate[1];
								}
								return best_score;
							}

						}
					}




				}

			}
		}
		else
		{
			//best_score_of_upper[floor] = 0;
			status = before_evaluation_ver4(priority_ver2, floor, step_count, my_turn);
			//final_hit = before_evaluation(board, priority, floor, step_count, my_turn);

			
			if (status != 0)//my_turn为假的时候不可能是最外层，因此少了一个if语句
			{
				//best_score = deepest(step_count, my_turn);
				return -infinity;
			}
			/*
			else
			{*/
			else
			{ 
				bool initialized = false;//false表示best_score还没有被赋值过

				for (int a = 0; a < 10; a++)
				{

					not_in_the_same_branch[floor - 1] = true;
					int raw = priority_ver2[a][0];
					int column = priority_ver2[a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((board[raw][column] != chess)
							&& (board[raw][column] != opponent_chess))
						{
							//初始化剪枝的参数
							if (floor - 2 >= 0)
							{
								best_score_of_upper_ver2[floor - 2] = -infinity;
								//not_in_the_same_branch[floor - 2] = true;
							}
							temp_blank = board[raw][column];
							board[raw][column] = chess;
							temp_score = 0;
							//下面这个是在测试的时候输出的，正式使用的时候可以关掉
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);
							if (temp_score == 0)
							{
								temp_score = Minimax3(step_count + 1, !my_turn, floor - 1);
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

								if (floor == FLOOR2)
									//如果是最外层，记录此时坐标
								{
									best_coordinate[0] = raw;
									best_coordinate[1] = column;
								}

								else
								{
									if ((best_score < best_score_of_upper_ver2[floor]) && not_in_the_same_branch[floor])//剪枝
									{  
										board[raw][column] = temp_blank;
										return -infinity;
									}
								}

							}


							else
							{


								if (temp_score < best_score)
								{
									best_score = temp_score;
									//这里没有那个最外层判定坐标的东西，因为最外层是不可能会出现传递min的情况的

									if (floor == FLOOR2)
										//如果是最外层，记录此时坐标
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
									}

									else
									{
										if ((best_score < best_score_of_upper_ver2[floor]) && not_in_the_same_branch[floor])//剪枝
										{
											board[raw][column] = temp_blank;
											return -infinity;
										}
									}
								}
							}

							board[raw][column] = temp_blank;

							if (best_score > best_score_of_upper_ver2[floor - 1])
							{
								best_score_of_upper_ver2[floor - 1] = best_score;
								not_in_the_same_branch[floor - 1] = false;
								if (best_score == 9375310)//测试用
									printf("\n");
							}
							if (best_score == -infinity)
							{
								return best_score;
							}

						}
					}

				}

			}
		}

	}
	//最底层↓
	else 
	{
		best_score = deepest(step_count, my_turn);
	}

	//最外层，将要返回一个最终决定的最优坐标
	if ((FLOOR2 - floor) == 0)
	{
		*coordinate = *best_coordinate;
		*(coordinate + 1) = *(best_coordinate + 1);
		best_score = evaluation_ver2(step_count, my_turn, coordinate[0], coordinate[1]);
	}



	//strncpy(board[best_coordinate[0]][best_coordinate[1]], temp_blank, 2);

	
	return best_score;
}


long int deepest(int step_count, bool my_turn)//最底层搜索单独提取出来了
{
	long int temp_score; 
	long int temp_score1, temp_score2;
	
	int raw, column;
	//这里删了一堆注释，要恢复的去看别的地方存档的minimax文件
	long int board_score = 0;

			//这个for循环是一开始就有的，别把这个给删了
	for (raw = 0; raw < 15; raw++)
	{
		for (column = 0; column < 15; column++)
		{
			if ((board[raw][column] != b)
				&& (board[raw][column] != w))
			{
				//temp_score = evaluation(board, step_count, my_turn, raw, column);

				temp_score1 = evaluation_ver2(step_count, my_turn, raw, column);
				temp_score2 = evaluation_ver2(step_count + 1, !my_turn, raw, column);
				/*
				temp_score1 = abs(temp_score1) * 1.5;
				temp_score2 = abs(temp_score2) * 0.75;
				temp_score = temp_score1 + temp_score2;
				*/
				temp_score = temp_score1 * 1.1 + temp_score2 * 0.9;
				board_score += temp_score;
				/*
				if (!initialized)
				{
					best_score = temp_score;
					initialized = true;
					best_raw = raw;
					best_column = column;

				}
				else
				{
					if (temp_score > best_score)
					{
						best_score = temp_score;

					}
				}
				*/

			}
		}
	}
	 
	

	return board_score;
}

void shallowest(int step_count, bool my_turn)//这个函数是用于只检索一层的情况
{
	long int temp_score1 = 0;
	long int temp_score2 = 0;

	bool initialized = false;
	long int temp_score = 0;
	long int best_score;
	int best_coordinate[2] = { 0,0 };
	//这个for循环是一开始就有的，别把这个给删了
	for (int raw = 0; raw < 15; raw++)
	{
		for (int column = 0; column < 15; column++)
		{
			if ((board[raw][column] != w)
				&& (board[raw][column] != b))
			{
				//temp_score = evaluation(board, step_count, my_turn, raw, column);

				temp_score1 = evaluation_ver2(step_count, my_turn, raw, column);
				temp_score2 = evaluation_ver2(step_count + 1, !my_turn, raw, column);

				temp_score1 = abs(temp_score1) * 1.1;
				temp_score2 = abs(temp_score2) * 0.9;
				temp_score = temp_score1 + temp_score2;

				if (!initialized)
				{ 
					best_score = temp_score;
					initialized = true;
					best_coordinate[0] = raw;
					best_coordinate[1] = column;

				}
				else
				{
					if (temp_score > best_score)
						//之所以不取等，是因为如果所有分支的分值都为0的话，就拿最开始出现的那个落点来下，因为最开始的落点是本层评分最高的
					{
						best_score = temp_score;					
						best_coordinate[0] = raw;
						best_coordinate[1] = column;
							

						
					}
				}
			}
		}
	}
	*coordinate = *best_coordinate;
	*(coordinate + 1) = *(best_coordinate + 1);
}