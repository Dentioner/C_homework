/*
 * =====================================================================================
 *
 *       Filename:  p295_t1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月05日 14时54分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<time.h>

int main()
{
	int i;
	struct timeval tv1, tv2;
	long ans;
//	gettimeofday(&tv1, NULL);
	clock_t ct1, ct2;
	double c_ans;
	ct1 = clock();
	for (i = 0; i < 1000000; i++)
	{
		asm volatile(
				"fldpi\n\t"
				"fldl2t\n\t"
				"fadd %st(0), %st(1)\n\t"
				);

	
	}

//	gettimeofday(&tv2, NULL);
	ct2 = clock();
//	ans = (long)(tv2.tv_usec - tv1.tv_usec);
	c_ans = (double)(ct2 - ct1)/CLOCKS_PER_SEC;
//	printf("time = %ld\ntime2 = %f\n", ans, c_ans);
	
	printf("time = %f\n", c_ans);
	return 0;
}
