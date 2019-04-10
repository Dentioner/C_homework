#pragma warning(disable:4996);
#include<stdio.h>
#include<math.h>

void insert_sort(int a[], int n)
{
	int i;
	int j, tmp;
	for (i = 1; i < n; i++)
	{
		tmp = a[i];
		j = i - 1;
		while (j >= 0 && a[j] > tmp)
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = tmp;
	}
}


int main()
{
	int a[10] = { 6, -5, 3, 1, -8, 7, 0, 2, 4, 9 };
	insert_sort(a, 10);
	for (int i = 0; i < 10; i++)
		printf("%d\t", a[i]);
	system("pause");
	return 0;
}