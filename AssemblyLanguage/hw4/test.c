#include<stdio.h>

int main()
{
	int str[11];	
	for (int i = 0; i<10; i++)
		scanf("%d", &str[i]);
	str[11] = '\0';

	for (int i = 0; i<10; i++)
		printf("%d", str[i]); 
	return 0;

}
