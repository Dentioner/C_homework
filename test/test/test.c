#include<stdio.h>
#include<string.h>
#include<math.h>
#include <ctype.h>


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
	double a = -5;
	double b = 1.5;
	double c = fmod(a, b);

	printf("%f mod %f = %f", a, b, c);
	return 0;
}