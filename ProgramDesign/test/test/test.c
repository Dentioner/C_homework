#include<stdio.h>
#include<string.h>
#include<math.h>
#include <ctype.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>



int main()
{
	int raw;
	int column;
	char column_c;
	while (1)
	{
		scanf("%d", &raw);
		while (getchar() != '\n')
			continue;
		column_c = toupper(getchar());
		column = column_c - 'A';
		raw = 15 - raw;
		printf("[%d][%d]\n", raw, column);
	}




	return 0;
}
