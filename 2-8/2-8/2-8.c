#include<stdio.h>

unsigned rightrot(unsigned x, int n);
//void bin(unsigned x);

int main()
{
	unsigned x;
	int n;
	unsigned output;
	printf("x=?\n");
	scanf("%ud", &x);

	printf("n=?\n");
	scanf("%d", &n);

	output = rightrot(x, n);
	printf("x = %u at first\nx = %u at last\n", x, output);



	return 0;
}

unsigned rightrot(unsigned x, int n)
{
	//假定最右边的是第0位
	//假定题目“向右移动n位”想表达的意思，是从右往左数n个位，即0~n-1这n位移动到最左边去
	unsigned output;
	unsigned a, b, c;
	int un_size = 0;
	//bin(x);
	a = ~(~0 << n);
	//bin(a);
	b = a & x;
	//bin(b);
	x = x >> n;
	//bin(x);

	c = x;
	while (c != 0)
	{
		//bin(c);
		c = c>> 1;
		un_size++;
		//system("pause");
	}
	

	b = b << (un_size);
	//bin(b);
	output = x | b;
	//bin(output);

	return output;
}

/*
void bin(unsigned x)
{

	int i, c = 0, a[100];

	while (x != 0)
	{
		i = x % 2;
		a[c] = i;
		c++;
		x = x / 2;
	}
	c--;//因为c代表存入数据的长度而下表范围是0~c-1
	for (; c >= 0; c--)//倒序输出即为这个数字对应的二进制
	{
		printf("%d", a[c]);
	}
	printf("\n");
	return 0;


}*/