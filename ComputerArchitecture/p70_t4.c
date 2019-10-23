/*
 * =====================================================================================
 *
 *       Filename:  p70_t4.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年10月23日 13时28分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */

#include<stdio.h>

typedef struct sa
{
	char s1;
	short s2;
	int s3;
	unsigned s4;
	long s5;
	long long s6;
	float s7;
	double s8;
	void * s9;
} sa;


int main()
{
	sa s[2];
	printf("start address:0x%x\n", &s[0]);
	printf("end address:0x%x\n", &s[1]);
	printf("sizeof(struct) = %d\n", sizeof(s[0]));
	printf("sizeof(char) = %d\n", sizeof(char));
	printf("sizeof(short) = %d\n", sizeof(short));
	printf("sizeof(int) = %d\n", sizeof(int));
	printf("sizeof(unsigned) = %d\n", sizeof(unsigned));

	printf("sizeof(long) = %d\n", sizeof(long));
	printf("sizeof(long long) = %d\n", sizeof(long long));

	printf("sizeof(float) = %d\n", sizeof(float));
	printf("sizeof(double) = %d\n", sizeof(double));
	printf("sizeof(void *) = %d\n", sizeof(void *));
	return 0;
}
