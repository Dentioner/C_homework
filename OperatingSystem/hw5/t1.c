/*
 * =====================================================================================
 *
 *       Filename:  t1.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2019年10月16日 20时54分31秒
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
#include<sys/time.h>
#include<sys/syscall.h>


#define LENGTH 10000000
unsigned array[LENGTH] = { 0 };

unsigned long long add_odd()
{
	int i;
	unsigned long long ans = 0;
	for (i = 0; i < LENGTH; i = i + 2)
	{
		ans += array[i];
	}

	return ans;
}

unsigned long long add_even()
{
	int i;
	unsigned long long ans = 0;
	for (i = 1; i < LENGTH; i = i + 2)
	{
		ans += array[i];
	}

	return ans;
}


int main()
{

	unsigned index;
	unsigned long long answer;
	unsigned long long r1, r2;
	pthread_t t1, t2;
	struct timeval tv1, tv2;
	long time_ans;
	
	//syscall(SYS_gettimeofday, &tv1, NULL);	
	gettimeofday(&tv1, NULL);


	for (index = 0; index < LENGTH; index++)
	{
		array[index] = index + 1;
	}

	//for(index = 0; index < LENGTH; index++)
	//	printf("array[%u]:%u\n", index, array[index]);
	pthread_create(&t1, NULL, add_odd, NULL);
	pthread_create(&t2, NULL, add_even, NULL);

	pthread_join(t1, &r1);
	pthread_join(t2, &r2);

	answer = r1 + r2;
	
	printf("answer is %llu\n", answer);
	
	//syscall(SYS_gettimeofday, &tv2, NULL);

	gettimeofday(&tv2, NULL);


	time_ans = (long)( tv2.tv_usec - tv1.tv_usec);
	printf("time: %li\n", time_ans);
	return 0;
}

