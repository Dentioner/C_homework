#include<stdio.h>
#include<string.h>
#include<math.h>
#include <ctype.h>
#include<time.h>
#include<stdlib.h>


void sta()
{
	static int a = 1;
	printf("a = %d\n", a);
	a++;
	printf("a++ = %d\n", a);
}