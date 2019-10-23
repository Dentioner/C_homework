/*
 * =====================================================================================
 *
 *       Filename:  p70_t3.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年10月22日 21时46分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dentioner Tchang, we980127@gmail.com
 *        Company:  University of Chinese Academy of Science
 *
 * =====================================================================================
 */
#include<stdio.h>

int leaf(int a, int b, int c, int d, int e, int f)
{
	int i;
	int y =  a+b+c+d+e ;
	for(i = 0; i < f; i++)
	{
		y++;
	}
	return y;
}


int main()
{
	int a, b, c, d, e, f;
	a = 1;
	b = 2;
	c = 3;
	d = 4;
	e = 5;
	f = 10;
	//int x = leaf(1, 2, 3, 4, 5, 6);	
	int x = leaf(a, b, c, d, e, f);
	printf("%d\n", x);
	return 0;
}
