#pragma warning(disable:4996);
#include<stdio.h>
#include<math.h>

int main()
{
	int a[100];
	
	int i = 0;
	char y;
	do
	{
		scanf("%d", &a[i++]);
	} while ((y = getchar()) != '(');// ���������ж��Ƿ������˻س�

	for (int p = 0; p < i; p++)
		printf("%d ", a[p]);
	system("pause");
	return 0;
}