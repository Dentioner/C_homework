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
	//�ٶ����ұߵ��ǵ�0λ
	//�ٶ���Ŀ�������ƶ�nλ���������˼���Ǵ���������n��λ����0~n-1��nλ�ƶ��������ȥ
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
	c--;//��Ϊc����������ݵĳ��ȶ��±�Χ��0~c-1
	for (; c >= 0; c--)//���������Ϊ������ֶ�Ӧ�Ķ�����
	{
		printf("%d", a[c]);
	}
	printf("\n");
	return 0;


}*/