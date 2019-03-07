/*
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

	 1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.

	 2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

	 3. The names of its contributors may not be used to endorse or promote
		products derived from this software without specific prior written
		permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
	 ACM Transactions on Modeling and
	 Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
	 ``Mersenne Twister: a 623-dimensionally equidistributed
	   uniform pseudorandom number generator''
	 ACM Transactions on Modeling and
	 Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/


#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include"head.h"
#include<time.h>
#include<stdlib.h>

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL
#define LM 0x7FFFFFFFULL
//ULL：unsigned long long
/* The array for the state vector */
static unsigned long long mt[NN];
/* mti==NN+1 means mt[NN] is not initialized  */

static int mti = NN + 1;

//unsigned long long ZobristTable[15][15][2];

/* initializes mt[NN] with a seed */
void init_genrand64(unsigned long long seed)
{
	mt[0] = seed;
	for (mti = 1; mti < NN; mti++)
		mt[mti] = (6364136223846793005ULL * (mt[mti - 1] ^ (mt[mti - 1] >> 62)) + mti);//这一行表达式没看懂是在算什么
}


/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void init_by_array64(unsigned long long init_key[],
	unsigned long long key_length)
{
	unsigned long long i, j, k;
	init_genrand64(19650218ULL);
	i = 1; j = 0;
	k = (NN > key_length ? NN : key_length);
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 62)) * 3935559000370003845ULL))
			+ init_key[j] + j; /* non linear */
		i++; j++;
		if (i >= NN) { mt[0] = mt[NN - 1]; i = 1; }
		if (j >= key_length) j = 0;
	}
	for (k = NN - 1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 62)) * 2862933555777941757ULL))
			- i; /* non linear */
		i++;
		if (i >= NN) { mt[0] = mt[NN - 1]; i = 1; }
	}

	mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */
}


/* generates a random number on [0, 2^64-1]-interval */
unsigned long long genrand64_int64(void)
{
	int i;
	unsigned long long x;
	static unsigned long long mag01[2] = { 0ULL, MATRIX_A };

	if (mti >= NN)
	{ /* generate NN words at one time */

		/* if init_genrand64() has not been called, */
		/* a default initial seed is used     */
		if (mti == NN + 1)
			init_genrand64(5489ULL);

		for (i = 0; i < NN - MM; i++)
		{
			x = (mt[i] & UM) | (mt[i + 1] & LM);
			mt[i] = mt[i + MM] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
		}
		for (; i < NN - 1; i++)
		{
			x = (mt[i] & UM) | (mt[i + 1] & LM);
			mt[i] = mt[i + (MM - NN)] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
		}

		x = (mt[NN - 1] & UM) | (mt[0] & LM);
		mt[NN - 1] = mt[MM - 1] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];

		mti = 0;
	}
	x = mt[mti++];

	x ^= (x >> 29) & 0x5555555555555555ULL;
	x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
	x ^= (x << 37) & 0xFFF7EEE000000000ULL;
	x ^= (x >> 43);

	return x;
}

int indexOf(char chess[3])
{
	char black[3] = "○";
	char white[3] = "●";
	//'o' represent white,'x' represent black
   // if (W_B == 'o')
	if (strncmp(chess, white, 2) == 0)
	{
		return 1;
	}
	else if ((strncmp(chess, black, 2) == 0))
	{
		return 0;
	}
}

void initTable(unsigned long long ZobristTable[15][15][2])
{
	unsigned long long init[4] = { 0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL }, length = 4;
	init_by_array64(init, length);
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				ZobristTable[i][j][k] = genrand64_int64();
				//test
				//printf("%ull\n", ZobristTable[i][j][k]);
			}
		}
	}
	//test
	//system("pause");
}

unsigned long long computeHash(char board[15][17][3], unsigned long long ZobristTable[15][15][2])
{
	unsigned long long h = 0;
	char black[3] = "○";
	char white[3] = "●";
	for (int raw = 0; raw < 15; raw++)
	{
		for (int column = 1; column < 16; column++)
		{
			if ((strncmp(board[raw][column], black, 2) != 0)
				&& (strncmp(board[raw][column], white, 2) != 0))
			{
				int W_B = indexOf(board[raw][column]);
				h ^= ZobristTable[raw][column][W_B];
			}
		}
	}
	return h;
}


/*
int main(void)
{
	char black[3] = "○";
	char white[3] = "●";
	char bl[3] = "┼";
	char board[15][16] = {
		"---------------",
		"---------------",
		"-----x-o-------",
		"-----o-x-------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------",
		"---------------" };
	char board2[15][17][3] =
	{
		{"15","┏","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┯","┓","15"},
		{"14","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","14"},
		{"13","┠","┼","┼","┼","○","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","13"},
		{"12","┠","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┼","┨","12"},
		{"11","┠","┼","┼","┼","┼","●","┼","┼","┼","┼","┼","┼","┼","┼","┨","11"},
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

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			for (int k = 0; k < 2; k++)
				putchar(board2[i][j][k]);
		}
		putchar('\n');
	}

	initTable();
	//unsigned long long hashValue = computeHash(board);
	unsigned long long hashValue = computeHash(board2);
	printf("The hash value is  : %llu\n", hashValue);
	//Move the 'x' to the board[1][5]
	//char piece = board[2][5];
	char piece2[3] = { '\0' };
	strncpy(piece2, board2[2][5], 2);
	board[2][5] = '-';
	strncpy(board2[2][5], bl, 2);
	hashValue ^= ZobristTable[2][5][indexOf(piece2)];
	//board[1][5] = piece;
	strncpy(board2[1][5], piece2, 2);
	hashValue ^= ZobristTable[1][5][indexOf(piece2)];

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			for (int k = 0; k < 2; k++)
				putchar(board2[i][j][k]);
		}
		putchar('\n');
	}
	system("pause");

	printf("The new hash vlaue is  : %llu\n", hashValue);
	//restore the 'x' to the board[2][5]
	//piece = board[1][5];
	strncpy(piece2, board2[1][5], 2);
	//board[1][5] = '-';
	strncpy(board2[1][5], bl, 2);
	hashValue ^= ZobristTable[1][5][indexOf(piece2)];
	//board[2][5] = piece;
	strncpy(board2[2][5], piece2, 2);
	hashValue ^= ZobristTable[2][5][indexOf(piece2)];
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			for (int k = 0; k < 2; k++)
				putchar(board2[i][j][k]);
		}
		putchar('\n');
	}
	system("pause");
	printf("after restore step, the hash vlaue is  : %llu\n", hashValue);
	system("pause");
	return 0;
}

*/