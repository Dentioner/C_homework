#include<stdio.h>

unsigned invert(unsigned x, int p, int n);
//void bin(unsigned x);

int main()
{
	unsigned x;
	int p, n;
	unsigned output;
	printf("x=?\n");
	scanf("%ud", &x);
	printf("p=?\n");
	scanf("%d", &p);
	printf("n=?\n");
	scanf("%d", &n);
	output = invert(x, p, n);
	printf("x = %u at first\nx = %u at last\n", x, output);
}

unsigned invert(unsigned x, int p, int n)
{
	//�ٶ���Ŀ���ӵ�pλ��ʼ��nλ���������˼���Ǵ���������n��λ����p~p+n-1��nλ
	//�ٶ����ұߵ��ǵ�0λ
	unsigned output;
	unsigned a;
	//bin(x);
	a = ~(~0 << n);
	//bin(a);
	a = a << p;
	//bin(a);
	output = x ^ a;
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