#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#include"head.h"

int board[15][15] = { 0 };
int coordinate[2] = { 0, 0 };
int w = 119;//白棋
int b = 98;//黑棋
int roaming = 0;//便于悔棋的，用来记录上一步的空格在哪个位置
unsigned long long ZobristTable[15][15][2];//梅森旋转的哈希键值表
unsigned long long hashValue = 0;//梅森旋转算法下，棋盘的哈希值
unsigned long long hashing_value2[depth_of_hashing][3] = { {0,0,0} };

int main()
{
	
	unsigned long long input = 9611115253591791470;
	initTable();
	int search = 0;
	/*
	while (true)
	{
		printf("input:");
		scanf("%lld", &input);
		hashValue = input;
		search = Searching_Hashing(2, 1, 100, 0);
		Searching_Hashing(2, 1, 100, 1);
	}
	*/
	unsigned long long a = input % 50000000;
	printf("%lld", a);


	return 0;
}