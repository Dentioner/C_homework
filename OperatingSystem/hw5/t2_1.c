/*
 * =====================================================================================
 *
 *       Filename:  t2_1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年10月17日 09时50分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<pthread.h>
#include<stdint.h>
#include<stdbool.h>
#include<sys/time.h>
#include<stdlib.h>
#include<time.h>

#define MAX 10000000
#define PIECE 100

int index = 0;
int index_even = 0;
int index_odd = 0;
bool flag[2] = {false, false};
int turn = 0;
uint64_t data[MAX];
//thread1

void* even(void * foo)
{
	int i;
	//flag[0] = true;
	//turn = 1;
	//while(index < MAX && index_even < MAX)
	do
	{
		flag[0] = true;
		turn = 1;
		while(flag[1] && turn == 1)
			;
		for(i = 0; i < PIECE; i++)
		{
			data[index] = index_even;
			index++;	
			index_even += 2;
		}
	
		flag[0] = false;	
	}while(index < MAX && index_even < MAX);
}	


//thread2

void* odd(void* foo)
{
	int i;
//	flag[1] = true;
//	turn = 0;
	//while(index < MAX && index_odd <MAX)
	do
	{
		flag[1] = true;
		turn = 0;
		
		while(flag[0] && turn == 0)
			;
		for(i = 0; i < PIECE; i++)
		{
			data[index] = index_odd + 1;
			index++;	
			index_odd += 2;
		}
	
		flag[1] = false;	
	}while(index < MAX && index_odd <MAX);

}

int main()
{
	uint64_t sum = 0;
	uint64_t max_sub = 0;
	uint64_t tmp_sub = 0;
	pthread_t t1, t2;
	struct timeval tv1, tv2;
	//long time_ans;
	int zero_counter = 0;
	clock_t start_time, end_time;
	double time_ans;	

	data[MAX-1] = 0;


	pthread_create(&t1, NULL, odd, NULL);
	pthread_create(&t2, NULL, even, NULL);


//	gettimeofday(&tv1, NULL);
	start_time = clock();	

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	end_time = clock();
//	gettimeofday(&tv2, NULL);


//	time_ans = (long)( tv2.tv_usec - tv1.tv_usec);
	time_ans = (double)(end_time - start_time)/CLOCKS_PER_SEC;

//	printf("time: %li\n", time_ans);
	printf("time: %f\n", time_ans);

for(int i = 0; i < MAX; i++)
	{
		sum += (uint64_t)data[i];
		if(i > 0)
		{
			tmp_sub = abs(data[i] - data[i-1]);
			if (tmp_sub > max_sub)
			{
				max_sub = tmp_sub;
			}
		}
	
	}
	printf("sum = %lu, max_sub = %lu\n", sum, max_sub);

	return 0;

}
