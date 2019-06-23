#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

//算法的时间复杂度为2n，即O(n)

int number0 = 0;
int number1 = 0;
int number2 = 0;
int total = 0;

void record_number()
{
	int ch;
	do
	{
		ch = getchar();
		if (ch == '0')
			number0++;
		else if (ch == '1')
			number1++;
		else if (ch == '2')
			number2++;
	} while (ch!= '\n');
	total = number0 + number1 + number2;

	return;
}

void print_flag()
{
	int print_counter = 0;
	while (print_counter < total)
	{
		for (int i = 0; i < number0; i++)
		{
			printf("0");
			if (print_counter < total - 1)
				printf(",");
			print_counter++;
		}
		for (int i = 0; i < number1; i++)
		{
			printf("1");
			if (print_counter < total - 1)
				printf(",");
			print_counter++;
		}
		for (int i = 0; i < number2; i++)
		{
			printf("2");
			if (print_counter < total - 1)
				printf(",");
			print_counter++;
		}
	}
	return;
}

int main()
{
	record_number();
	print_flag();
	return 0;
}