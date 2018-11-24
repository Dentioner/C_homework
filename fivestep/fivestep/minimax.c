#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"head.h"
#include<time.h>

//��Ϊȥ����ϣ�İ汾�������Աȹ�ϣ���Ǹ��汾�Ƿ�����
long int Minimax2(char board[][17][3], int step_count,
	bool my_turn, bool ai_first,
	int floor, int coordinate[], long int best_score_of_upper[], int priority[][30][2], bool not_in_the_same_branch[],
	unsigned long long hashValue, unsigned long long ZobristTable[15][15][2], unsigned long long hashing_value2[depth_of_hashing][3],
	int fatal_priority[][32][2], long int fatal_best_score_of_upper[], bool fatal_not_in_the_same_branch[])
{
	char black[2] ="��";
	
	char white[2] = "��";
	char temp_blank[2];//���������ԭ���̣��൱�ڻ���һ����
	char *chess;
	char *opponent_chess;
	//bool final_hit = false;
	int status = 0;
	//0��ʾ��ǰ�������
	//1��ʾ��ǰ��ַ����ҷ�����
	//2��ʾ��ǰ���û�з����ҷ����壬���Ƿ��ֶԷ�����
	int priority_ver2[10][2] = { {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} };
	//bool valid_coordinate = false;
	//�������ڽ���ai���֡��غ����롰�Ƿ����ҷ��غϡ��Ĺ�ϵ

	//����������������������ϵ���е��������õģ�����������ʱ��Ҫע�͵�
	if (coordinate[0] == 14 && coordinate[1] == 15 && floor == FLOOR)
	{
		printf("\n");
	}


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
			//�Ƚ����ȵ���Щ���ҵ����ݹ�
			//status = before_evaluation_ver2(board, priority, floor, step_count, my_turn);
			status = before_evaluation_ver3(board, priority_ver2, floor, step_count, my_turn, &chess);


			//�������˫���forѭ�����ڲ��Ե�ʱ������ģ���ʽʹ�õ�ʱ����Թص�
			/*
			if (coordinate[0] == 4 && coordinate[1] == 13)
			{
				for (int test_raw = 0; test_raw < 10; test_raw++)
				{
					for (int test_raw2 = 0; test_raw2 < 30; test_raw2++)
					{

						printf("%d,", priority[test_raw][test_raw2][0]);
						printf("%d", priority[test_raw][test_raw2][1]);
						printf(" ");
					}
					printf("\n");
				}
			}
			*/



			//printf("������Ƿ����ϵ����õ�\n");



			if (status != 0)
			{


				long int temp_score1 = 0;
				long int temp_score2 = 0;
				int best_raw = 0;
				int best_column = 1;
				bool initialized = false;
				//int floor_vcx = FLOOR_VCX;//�������ɱר��floor
				//int fatal_best_coordinate[2] = { 0,1 };
				//����ѵĵ�����ֵ
				
					temp_score = 0;
					if (temp_score == 0)

					{


						//���forѭ����һ��ʼ���еģ���������ɾ��
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
		//֮���Բ�ȡ�ȣ�����Ϊ������з�֧�ķ�ֵ��Ϊ0�Ļ��������ʼ���ֵ��Ǹ�������£���Ϊ�ʼ������Ǳ���������ߵ�
										{
											best_score = temp_score;
											if (floor == FLOOR)
												//���������㣬��¼��ʱ����
											{
												best_coordinate[0] = raw;
												best_coordinate[1] = column;
												best_raw = raw;
												best_column = column;

											}
										}
									}
									/*
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
											best_coordinate[0] = raw;
											best_coordinate[1] = column;
										}
							
									}
									*/
								}
							}
						}
						


					}
					

					else
					{
					
						best_score = temp_score;
					}






				
				





			}
		



			else
			{

				bool initialized = false;//false��ʾbest_score��û�б���ֵ��
				//for (int a = 0; a < 30; a++)
				//{

				//���ԣ���ӡpriority����
				/*
				for (int ceshi = 0; ceshi < 10; ceshi++)
				{
					printf("���꣺(%d, %d)\n", priority_ver2[ceshi][0], priority_ver2[ceshi][1]);
				}
				system("pause");
				*/
				for (int a = 0; a < 10; a++)
				{
					not_in_the_same_branch[floor - 1] = true;//�ж��Ƿ���ͬһ��֧�У��������֦
					int raw = priority_ver2[a][0];
					int column = priority_ver2[a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((strncmp(board[raw][column], chess, 2) != 0)
							&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
						{
							//��ʼ����֦�Ĳ���
							if (floor - 2 >= 0)
							{
								best_score_of_upper[floor - 2] = 89999900;
								//not_in_the_same_branch[floor - 2] = true;
							}
							strncpy(temp_blank, board[raw][column], 2);
							strncpy(board[raw][column], chess, 2);
							//hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
							//�����⼸�����ڲ��Ե�ʱ��ʹ�õģ���ʽʹ�õ�ʱ��ص�

							//DrawBoard(board, 15, 0, 2, coordinate, step_count);


							long int temp_score1 = 0;//Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
							long int temp_score2 = 0;//Searching_Hashing2(hashing_value2, ZobristTable, step_count + 1, hashValue, !my_turn, 0, false);
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
							//�����������ڲ��Ե�ʱ��ʹ�õģ���ʽʹ�õ�ʱ��ص�
							//DrawBoard(board, 15, 0, 2, coordinate, step_count);


							//�����ǴӾ���������������


							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;

								if (floor == FLOOR)
									//���������㣬��¼��ʱ����
								{
									best_coordinate[0] = raw;
									best_coordinate[1] = column;
								}
								//�����֦���޸�

								else
								{
									if ((best_score > best_score_of_upper[floor]) && (not_in_the_same_branch[floor]))//��֦
									{
										strncpy(board[raw][column], temp_blank, 2);
										return 89999900;
									}

								}

							}

							else
							{



								if (temp_score > best_score)//��ȡ�ȵ�ԭ�������н���
								{
									best_score = temp_score;

									//valid_coordinate = verify_coordinate(board, raw, column, chess, opponent_chess);
									//if ((floor == FLOOR)&&valid_coordinate)
									if (floor == FLOOR)
										//���������㣬��¼��ʱ����
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
									}
									//�����֦���޸�

									else
									{
										if ((best_score > best_score_of_upper[floor]) && (not_in_the_same_branch[floor]))//��֦
										{
											strncpy(board[raw][column], temp_blank, 2);
											return 89999900;
										}

									}
								}
								//��ԭ
							}
							strncpy(board[raw][column], temp_blank, 2);
							//Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, temp_score, true);
							//hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
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

			}
		}
		else
		{
			//best_score_of_upper[floor] = 0;
			//status = before_evaluation_ver2(board, priority, floor, step_count, my_turn);
		
			status = before_evaluation_ver3(board, priority_ver2, floor, step_count, my_turn, &chess);


			//�������˫���forѭ�����ڲ��Ե�ʱ������ģ���ʽʹ�õ�ʱ����Թص�
			/*
			if (coordinate[0] == 4 && coordinate[1] == 13)
			{
				for (int test_raw = 0; test_raw < 10; test_raw++)
				{
					for (int test_raw2 = 0; test_raw2 < 30; test_raw2++)
					{

						printf("%d,", priority[test_raw][test_raw2][0]);
						printf("%d", priority[test_raw][test_raw2][1]);
						printf(" ");
					}
					printf("\n");
				}
			}
			*/
			//printf("������Ƿ����ϵ����õ�\n");


			if (status != 0)
			{
				bool initialized = false;
				long int temp_score1 = 0;
				long int temp_score2 = 0;
				int best_raw = 0;
				int best_column = 1;
				
				
				
					temp_score = 0;
					if (temp_score == 0)

					{
						

						//�������forѭ����ɾ��
						for (int raw = 0; raw < 15; raw++)
						{
							for (int column = 1; column < 16; column++)
							{
								if ((strncmp(board[raw][column], chess, 2) != 0)
									&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
								{
									temp_score1 = evaluation(board, step_count, my_turn, raw, column);
									temp_score2 = evaluation(board, step_count + 1, !my_turn, raw, column);
									temp_score1 = abs(temp_score1) * 1.5;
									temp_score2 = abs(temp_score2) * 0.75;
									temp_score = temp_score1 + temp_score2;
									temp_score = -temp_score;
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
												//���������㣬��¼��ʱ����
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
						//best_score = temp_score1 + temp_score2;
						best_score = temp_score;
					}

				





			}
			else
			{


				bool initialized = false;//false��ʾbest_score��û�б���ֵ��
				
				for (int a = 0; a< 10; a++)
				{

					not_in_the_same_branch[floor - 1] = true;
					int raw = priority_ver2[a][0];
					int column = priority_ver2[a][1];
					if ((raw != 0) || (column != 0))
					{
						if ((strncmp(board[raw][column], chess, 2) != 0)
							&& (strncmp(board[raw][column], opponent_chess, 2) != 0))
						{
							//��ʼ����֦�Ĳ���
							if (floor - 2 >= 0)
							{
								best_score_of_upper[floor - 2] = -89999900;
								//not_in_the_same_branch[floor - 2] = true;
							}
							strncpy(temp_blank, board[raw][column], 2);
							strncpy(board[raw][column], chess, 2);
							//hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
							long int temp_score1 = 0;//Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, 0, false);
							long int temp_score2 = 0;//Searching_Hashing2(hashing_value2, ZobristTable, step_count + 1, hashValue, !my_turn, 0, false);

							//����������ڲ��Ե�ʱ������ģ���ʽʹ�õ�ʱ����Թص�
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


							//�����ǴӾ���������������
							//if ((temp_score != 0) && (best_score == 0))
		//֮���԰������temp_score != 0Ҳ��ȥ��������Ϊ������һ��֦��һ������0��������ô����ֱ�Ӹ���bestscore
		//������һ����ԽСԽ�ã����֮ǰ��0��֦����ȴ������ˣ��ǲ������
		//������������һ�����Ϊ0�ĵ㣬���һ���ѵ��˴��Ϊ100�ĵ㣬��ȻӦ������һ�㴫��0�ֵģ���������������߼��жϣ�����ȥ��100��
		//����ʲô����дҲ���У���Ϊ����и��֣��ڡ�������Сֵ�������������޷���Ϊ����ֵ
									//����Ӧ�ò���״̬���ķ�������������һ��ֵ��ʱ��֤�ḳ��bests
							if (!initialized)
							{
								best_score = temp_score;
								initialized = true;

								if (floor == FLOOR)
									//���������㣬��¼��ʱ����
								{
									best_coordinate[0] = raw;
									best_coordinate[1] = column;
								}

								else
								{
									if ((best_score < best_score_of_upper[floor]) && not_in_the_same_branch[floor])//��֦
									{
										strncpy(board[raw][column], temp_blank, 2);
										return -89999900;
									}
								}

							}


							else
							{


								if (temp_score < best_score)
								{
									best_score = temp_score;
									//����û���Ǹ�������ж�����Ķ�������Ϊ������ǲ����ܻ���ִ���min�������

									if (floor == FLOOR)
										//���������㣬��¼��ʱ����
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
									}

									else
									{
										if ((best_score < best_score_of_upper[floor]) && not_in_the_same_branch[floor])//��֦
										{
											strncpy(board[raw][column], temp_blank, 2);
											return -89999900;
										}
									}
								}
							}

							strncpy(board[raw][column], temp_blank, 2);
							//	Searching_Hashing2(hashing_value2, ZobristTable, step_count, hashValue, my_turn, temp_score, true);
							//	hashValue ^= ZobristTable[raw][column - 1][(step_count % 2)];
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

	}
	//��ײ��
	else
	{
		long int temp_score1 = 0;
		long int temp_score2 = 0;
		int best_raw = 0;
		int best_column = 1;
		bool initialized = false;//false��ʾbest_score��û�б���ֵ��
		if (my_turn)
		{
			temp_score = 0;
			if (temp_score == 0)

			{
				//���forѭ����һ��ʼ���еģ���������ɾ��
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
									if (floor == FLOOR)
										//���������㣬��¼��ʱ����
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
										best_raw = raw;
										best_column = column;

									}
								}
							}
							/*
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
						
							}
							*/
						}
					}
				}
				

			}
		

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
			if (temp_score == 0)

			{
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
							temp_score = -temp_score;
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
										//���������㣬��¼��ʱ����
									{
										best_coordinate[0] = raw;
										best_coordinate[1] = column;
										best_raw = raw;
										best_column = column;

									}
								}
							}
							
							/*
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
								
								}
					
							}
							*/
						}

					}
				}
		

			}
			else
			{
				//best_score = temp_score1 + temp_score2;
				best_score = temp_score;
			}

		}
	}

	//����㣬��Ҫ����һ�����վ�������������
	if ((FLOOR - floor) == 0)
	{
		*coordinate = *best_coordinate;
		*(coordinate + 1) = *(best_coordinate + 1);
		best_score = evaluation(board, step_count, my_turn, coordinate[0], coordinate[1]);
	}



	//strncpy(board[best_coordinate[0]][best_coordinate[1]], temp_blank, 2);


	return best_score;
}
