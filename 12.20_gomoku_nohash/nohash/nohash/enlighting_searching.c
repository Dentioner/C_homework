#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"head.h"
#include<stdlib.h>

extern int board[15][15];
extern int w;//白棋
extern int b;//黑棋
extern int coordinate[2];
extern bool show_me_the_array;//测试用
extern bool ai_first;
extern long int empty_score_total_black[15][15];
extern long int empty_score_total_white[15][15];
extern int  temp_point[2];
//启发式搜索，按照优先级顺序，对分支进行排序，提高剪枝速度
//见https://github.com/lihongxun945/gobang/blob/master/src/ai/board.js

int before_evaluation_ver4_5(int priority_ver2[][2], int step_count)
{
	long int temp_priority[255][3] = { 0 };//先将255个点的评分全部存下来
	int my_raw = 0;
	int my_column = 0;

	int index = 0;
	int i = 0;
	long int my_value = 0;
	long int opponent_value = 0;
	long int temp_value = 0;
	int status = 0;
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五，不包括对方连五
	int array_size = 1;



	if (step_count % 2)//说明是白棋方访问此函数
	{
		for (my_raw = 0; my_raw < 15; my_raw++)
		{
			for (my_column = 0; my_column < 15; my_column++)
			{
				if ((board[my_raw][my_column] != b)
					&& (board[my_raw][my_column] != w))
				{
					//temp_value = empty_score_total[my_raw][my_column];
					my_value = empty_score_total_white[my_raw][my_column];
					//my_value = labs(my_value);
					opponent_value = empty_score_total_black[my_raw][my_column];
					//opponent_value = labs(opponent_value);
					temp_value = my_value + opponent_value;

					if (temp_value != 0)
					{
						if (my_value >= Consecutive_Five)//发现我方连五，返回
						{
							status = 1;
							return status;
						}

						else//否则继续录入数据
						{//这里写排序的算法
							temp_priority[index][0] = temp_value;
							temp_priority[index][1] = my_raw;
							temp_priority[index][2] = my_column;
							index++;


						}
					}
				}
			}
		}
	}

	else//说明是黑棋方访问的此函数
	{
		for (my_raw = 0; my_raw < 15; my_raw++)
		{
			for (my_column = 0; my_column < 15; my_column++)
			{
				if ((board[my_raw][my_column] != b)
					&& (board[my_raw][my_column] != w))
				{
					//test
					//if (my_raw == 5 && my_column == 10)
						//printf("\n");

					//temp_value = empty_score_total[my_raw][my_column];
					my_value = empty_score_total_black[my_raw][my_column];
					//my_value = labs(my_value);
					opponent_value = empty_score_total_white[my_raw][my_column];
					//opponent_value = labs(opponent_value);
					temp_value = my_value + opponent_value;

					if (temp_value != 0)
					{
						if (my_value >= Consecutive_Five)//发现我方连五，返回
						{
							status = 1;
							return status;
						}

						else//否则继续录入数据
						{//这里写排序的算法
							temp_priority[index][0] = temp_value;
							temp_priority[index][1] = my_raw;
							temp_priority[index][2] = my_column;
							index++;
						}
					}
				}
			}
		}
	}
	quick_sort(temp_priority, 0, index - 1);//index的值是第一个0元素的位置

	for (i = 0; (i < index) && (i < 10); i++)//将temp的前十大的坐标赋值给外面的prioriy_ver2
	{
		priority_ver2[i][0] = temp_priority[i][1];
		priority_ver2[i][1] = temp_priority[i][2];
		/*
		if (temp != *chess_pointer)
		{
			printf(" ");
		}
		*/
	}


	if (show_me_the_array)
	{

		for (int ceshi = 0; ceshi < 10; ceshi++)
		{
			printf("评分：%d，坐标：(%d, %d)\n", temp_priority[ceshi][0], temp_priority[ceshi][1], temp_priority[ceshi][2]);
		}
		//system("pause");
	}

	return status;
}

int before_evaluation_ver6(int priority_ver2[][2], int step_count)
{
	int my_raw, my_column;
	long int my_value, opponent_value;
	long int temp_value;
	int status = 0;
	//0表示当前棋局正常
	//1表示当前棋局发现我方连五，不包括对方连五
	int node_number = 0;//记录形成的链表节点数
	int index;
	Node * head = NULL;
	Node * prev, *current, *post;
	Node * insert_node;


	if (step_count % 2)//说明是白棋方访问此函数
	{
		for (my_raw = 0; my_raw < 15; my_raw++)
		{
			for (my_column = 0; my_column < 15; my_column++)
			{
				if ((board[my_raw][my_column] != b)
					&& (board[my_raw][my_column] != w))
				{
					my_value = empty_score_total_white[my_raw][my_column];
					opponent_value = empty_score_total_black[my_raw][my_column];
					temp_value = my_value + opponent_value;
					if (temp_value != 0)
					{
						if (my_value >= Consecutive_Five)//发现我方连五，返回
						{
							status = 1;
							return status;
						}

						else//否则继续录入数据
						{//这里写排序的算法

							insert_node = (Node *)malloc(sizeof(Node));
							insert_node->score = temp_value;
							insert_node->raw = my_raw;
							insert_node->column = my_column;
							insert_node->next = NULL;//这五行是制造一个待插入的节点



							index = 0;//index记录遍历节点的次数，即和节点比较了几次
							if (head == NULL)//初始化整个链表，此时一个节点都没有
							{
								head = insert_node;
								node_number++;
							}
							else
							{
								current = head;//首先将第一个节点当做当前节点
								prev = NULL;
								post = head->next;//紧随其后的节点就是head之后的那个节点
								//上面这三行是将待比较的链表初始化


								while (index < node_number && current != NULL)
								{//如果当前比较的次数比节点总数小，并且当前节点的指针不是空指针
									if (insert_node->score >= current->score)
									{//如果待插入的节点的分值比当前节点的分值高
										if (index == 0)//如果待插入的节点是最高分
										{
											insert_node->next = head;//将链表头的下一个地址指向当前表头
											head = insert_node;//链表头就改成待插入的节点
											node_number++;//节点数+1
											break;
										}
										else
										{
											insert_node->next = current;//将待插入节点的下一个地址指向当前节点
											prev->next = insert_node;//将前一个节点的下一个地址指向待插入节点
											node_number++;//节点数+1
											break;//跳出循环
										}
									}
									else
									{//如果待插入的节点的分值比当前节点的分值低
										prev = current;//当前节点的地址赋给前一个节点，即前移一位
										current = post;//后一个节点的地址赋给当前节点，即前移一位
										if (post != NULL)
											post = post->next;//后一个节点指向的后后一个节点的地址赋给后一个节点，即前移一位
										index++;//计数，看看比较了几个节点了
									}
								}
								if (index == node_number)//如果比较次数等于节点数，说明这个分数是最小的，应该放在链表最后
								{
									prev->next = insert_node;//将当前节点的最后地址指向待插入的节点
									node_number++;
								}
							}
						}
					}
				}
			}
		}
	}

	else//说明是黑棋方访问的此函数
	{
		for (my_raw = 0; my_raw < 15; my_raw++)
		{
			for (my_column = 0; my_column < 15; my_column++)
			{
				if ((board[my_raw][my_column] != b)
					&& (board[my_raw][my_column] != w)
					&& (!detect_forbidden_step(my_raw, my_column)))
				{
					my_value = empty_score_total_black[my_raw][my_column];
					opponent_value = empty_score_total_white[my_raw][my_column];
					temp_value = my_value + opponent_value;
					if (temp_value != 0)
					{
						if (my_value >= Consecutive_Five)//发现我方连五，返回
						{
							status = 1;
							return status;
						}

						else//否则继续录入数据
						{//这里写排序的算法

							insert_node = (Node *)malloc(sizeof(Node));
							insert_node->score = temp_value;
							insert_node->raw = my_raw;
							insert_node->column = my_column;
							insert_node->next = NULL;//这五行是制造一个待插入的节点



							index = 0;//index记录遍历节点的次数，即和节点比较了几次
							if (head == NULL)//初始化整个链表，此时一个节点都没有
							{
								head = insert_node;
								node_number++;
							}
							else
							{
								current = head;//首先将第一个节点当做当前节点
								prev = NULL;
								post = head->next;//紧随其后的节点就是head之后的那个节点
								//上面这三行是将待比较的链表初始化

								while (index < node_number && current != NULL)
								{//如果当前比较的次数比节点总数小，并且当前节点的指针不是空指针
									if (insert_node->score >= current->score)
									{//如果待插入的节点的分值比当前节点的分值高
										if (index == 0)//如果待插入的节点是最高分
										{
											insert_node->next = head;//将链表头的下一个地址指向当前表头
											head = insert_node;//链表头就改成待插入的节点
											node_number++;//节点数+1
											break;
										}
										else
										{
											insert_node->next = current;//将待插入节点的下一个地址指向当前节点
											prev->next = insert_node;//将前一个节点的下一个地址指向待插入节点
											node_number++;//节点数+1
											break;//跳出循环
										}
									}
									else
									{//如果待插入的节点的分值比当前节点的分值低
										prev = current;//当前节点的地址赋给前一个节点，即前移一位
										current = post;//后一个节点的地址赋给当前节点，即前移一位
										if (post != NULL)
											post = post->next;//后一个节点指向的后后一个节点的地址赋给后一个节点，即前移一位
										index++;//计数，看看比较了几个节点了
									}
								}
								if (index == node_number)//如果比较次数等于节点数，说明这个分数是最小的，应该放在链表最后
								{
									prev->next = insert_node;//将当前节点的最后地址指向待插入的节点
									node_number++;
								}
							}
						}
					}
				}
			}
		}
	}

	current = head;
	for (index = 0; (current != NULL) && (index < 10); index++)//将temp的前十大的坐标赋值给外面的prioriy_ver2
	{

		priority_ver2[index][0] = current->raw;
		priority_ver2[index][1] = current->column;
		current = current->next;
	}

	//释放内存

	while (head != NULL)
	{
		current = head;
		head = current->next;
		free(current);
	}
	return status;
}

void refresh_score(int step_count, bool my_turn)//重新刷新刚落下的子的四个方向上6个单位以内的空格的分数，如果速度过慢，就修改为4个单位以内的空格
{

	int horizon[2] = { 0,1 };//水平方向单位矢量
	int perpendicular[2] = { 1,0 };//垂直方向单位矢量
	int up_right_down_left[2] = { -1,1 };//右上左下方向单位矢量
	int up_left_down_right[2] = { 1,1 };//左上右下方向单位矢量
	/*
	for (i = -6, j = -6; i < 6 && j < 6; i++, j++)
	{
		re_calculate(horizon, i, j, step_count, my_turn);//刷新水平方向的空格的分数
		re_calculate(perpendicular, i, j, step_count, my_turn);//刷新竖直方向的空格的分数
		re_calculate(up_right_down_left, i, j, step_count, my_turn);//刷新右上左下方向的空格的分数
		re_calculate(up_left_down_right, i, j, step_count, my_turn);//刷新左上右下方向的空格的分数

	}
	*/
	//if (temp_point[0] == 8 && temp_point[1] == 6)
		//DrawBoard(0, 2, step_count);

	re_calculate(horizon, step_count, my_turn);//刷新水平方向的空格的分数
	re_calculate(perpendicular, step_count, my_turn);//刷新竖直方向的空格的分数
	re_calculate(up_right_down_left, step_count, my_turn);//刷新右上左下方向的空格的分数
	re_calculate(up_left_down_right, step_count, my_turn);//刷新左上右下方向的空格的分数
}

void re_calculate(int vector[], int step_count, bool my_turn)
{
	/*
	int raw, column;
	long int value;
	raw = temp_point[0] + vector[0] * i;
	column = temp_point[1] + vector[1] * j;
	value = evaluation_ver2(step_count, my_turn, raw, column);
	empty_score_total[raw][column] = value;
	*/
	int i, j;
	int raw, column, point;
	long int white_value, black_value;
	for (i = -6, j = -6; i < 7 && j < 7; i++, j++)
	{
		raw = temp_point[0] + vector[0] * i;
		column = temp_point[1] + vector[1] * j;
		if (raw >= 0 && raw <= 14 && column >= 0 && column <= 14)
		{
			point = board[raw][column];
			if (point != w && point != b && (raw != temp_point[0] || column != temp_point[1]))
			{//判断条件为：不是棋子，且不是刚刚下落的那个点
//之所以添加后面那个条件是因为在撤回棋子并重新让分数变回去的时候，刚刚下落的那个位置也会被重新算一遍，但是实际上那个位置不用算
				black_value = evaluation(2, true, raw, column);//将step_count强行令为偶数2，保证是黑子得分，并将my_turn强行置为true，保证输出得分为正数
				white_value = evaluation(3, true, raw, column);//将step_count强行令为奇数3，保证是白子得分，并将my_turn强行置为true，保证输出得分为正数
				empty_score_total_black[raw][column] = black_value;
				empty_score_total_white[raw][column] = white_value;
				//if (empty_score_total_white[4][12] == 10000)
					//printf("test\n");
			}
		}

	}
}

void quick_sort(long int temp_priority[][3], int l, int r)//对二维数组进行降序的快速排序
{//此函数参考https://blog.csdn.net/morewindows/article/details/6684558
	if (l < r)
	{

		int i = l, j = r;
		int x[3];
		x[0] = temp_priority[l][0];
		x[1] = temp_priority[l][1];
		x[2] = temp_priority[l][2];
		while (i < j)
		{
			while (i < j && temp_priority[j][0] < x[0])
				j--;
			if (i < j)
			{
				temp_priority[i][0] = temp_priority[j][0];
				temp_priority[i][1] = temp_priority[j][1];
				temp_priority[i][2] = temp_priority[j][2];

				//system("pause");
				i++;
			}


			while (i < j && temp_priority[i][0] >= x[0])
				i++;
			if (i < j)
			{
				temp_priority[j][0] = temp_priority[i][0];
				temp_priority[j][1] = temp_priority[i][1];
				temp_priority[j][2] = temp_priority[i][2];

				//system("pause");
				j--;
			}


		}
		temp_priority[i][0] = x[0];
		temp_priority[i][1] = x[1];
		temp_priority[i][2] = x[2];
		quick_sort(temp_priority, l, i - 1);
		quick_sort(temp_priority, i + 1, r);
	}
}
