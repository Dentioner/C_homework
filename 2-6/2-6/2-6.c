#include<stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y);
//void bin(unsigned x);

int main()
{
	unsigned x, y;
	int p, n;
	unsigned output;
	printf("x=?\n");
	scanf("%ud", &x);
	printf("p=?\n");
	scanf("%d", &p);
	printf("n=?\n");
	scanf("%d", &n);
	printf("y=?\n");
	scanf("%ud", &y);
	output = setbits(x, p, n, y);
	printf("x = %u at first\nx = %u at last\n", x, output);

	return 0;
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	unsigned output;
	unsigned a, b;
	//�ٶ����ұߵ��ǵ�0λ
	//�ٶ���Ŀ���ӵ�pλ��ʼ��nλ���������˼���Ǵ���������n��λ����p~p+n-1��nλ
	//�ٶ���y�����ұ�nλ��ָ����0~n-1λ
	a = ~(~0 << n);
	//bin(a);
	a = ~(a << p);
	//bin(a);
	x = x & a;
	//bin(x);

	b = ~(~0 << n);
	//bin(b);
	y = y & b;
	//bin(y);

	y = y << p;
	//bin(y);
	output = x | y;
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
	c--;//��Ϊc����������ݵĳ��ȶ��±�Χ��0~c-1
	for (; c >= 0; c--)//���������Ϊ������ֶ�Ӧ�Ķ�����
	{
		printf("%d", a[c]);
	}
	printf("\n");
	return 0;

	
}*/