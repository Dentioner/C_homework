#include<stdio.h>

void reverse(int s[], int length)
{
	int c, i, j;
	for (i = 0, j = length - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}


}




int itob(int n, int s[], int b)
{
	int yu_shu = 0;
	int shang = 0;
	int i = 0;
	while (n != 0)
	{
		shang = n / b;
		yu_shu = n % b;
		s[i] = yu_shu;
		i++;
		n = shang;
	}
	


	reverse(s, i);

	return i;
}




int main()
{
	int n, b;
	int index = 0;
	int length = 0;
	int s[100] = {'\0'};
	printf("打算用几进制？\n");
	scanf("%d", &b);
	printf("输入一个十进制的数\n");
	scanf("%d", &n);
	printf("n = %d, b = %d\n", n, b);
	length = itob(n, s, b);
	printf("转换后，%d进制下，n = ", b);
	for (index = 0; index < length; index++)
	{
		
		printf("%d ",s[index]);
	}
	printf("\n");


	return 0;
}