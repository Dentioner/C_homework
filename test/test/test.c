#include<stdio.h>
#include<string.h>
#include<math.h>
#include <ctype.h>
#include<time.h>
#include<stdlib.h>

void myprintf(int array[], int length)
{
	int index = 0;
	for (index = 0; index < length; index++)
	{
		printf("%d,", array[index]);
	}
	printf("\n");
}



int main()
{	
	unsigned long long base, r1, r2, p;
	//base = 7438147750787157163 ^ 3862249798930641332 ^ 10815312636510328870 ^ 10592706450534565444;
	base = 0 ^ 3862249798930641332 ^ 10815312636510328870 ^ 10592706450534565444;
	r1 = base ^ 12314609993579474774 ^ 1845293119018433391 ^ 10427377732172208413;
	r2 = base ^ 10427377732172208413 ^ 1845293119018433391 ^ 12314609993579474774;
	p = base ^ 10427377732172208413 ^ 1845293119018433391;
	printf("base = %llu\nr1 = %llu\nr2 = %llu\n", base, r1, r2);
	printf("p = %llu\n", p);
	return 0;
}