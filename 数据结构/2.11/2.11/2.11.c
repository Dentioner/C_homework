#pragma warning(disable:4996);
#define MaxLength 100
#include<stdio.h>
#include<stdlib.h>
#include<time.h>//
int main()
{
	
	int i = 0;
	int*va;
	int *a = NULL, *b;
	int e;
	char ch;

	double start_time, end_time;//
	
	va = (int*)malloc(MaxLength * sizeof(int));


	do
	{

		scanf("%d", &(va[i++]));
	} while ((ch = getchar())!='\n');
	start_time = clock();//
	e = va[i - 1];

	for (int p = 0; p < i; p++)
	{
		if (va[p] >= va[i - 1])
		{
			a = &va[p];

			break;
		}
	}

	for (b = &(va[i - 1]); b >= a; --b)
		*(b + 1) = *b;
	*a = e;


	for (int p = 0; p < i-1; p++)
		printf("%d ", va[p]);
	printf("%d", va[i - 1]);
	end_time = clock();//
	printf("\ntime = %f\n", end_time - start_time);//
	system("pause");//
	return 0;
}