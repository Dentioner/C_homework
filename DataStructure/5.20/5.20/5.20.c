#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10

typedef enum { OK, ERROR } Status;
typedef int ElemType;

typedef struct 
{
	ElemType *base;
	int dim;//ά�Ⱦ��Ǳ�Ԫ����
	//int *bounds;
	//int *constants;
	int bound_length;//ÿһά���Ⱦ�����ߴ���
	int elemtotal;
}Array;

Array mx;

Status InitArray();
Status printa();
void print_element(int ex, int base);

int main()
{
	if (InitArray() == ERROR)
		exit(1);
	

	printa();

	system("pause");
	return 0;

}

Status InitArray()
{
	int dim, bound_length;
	int elem_max_num = MAXLENGTH;
	int ch, number;
	int dim_counter = 0;
	//int number_counter = 0;
	scanf("%d;%d;", &dim, &bound_length);
	mx.dim = dim;
	//mx.bounds = (int*)malloc(dim * sizeof(int));
	//if (!mx.bounds)
	//	exit(1);
	mx.bound_length = bound_length;
	mx.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
	mx.elemtotal = 0;
	if (!mx.base)
		exit(ERROR);
	ch = getchar();
	while (ch != EOF && ch != '\n')
	{
		if (ch == '[')
		{
			dim_counter++;
		}
		else if (ch == ']')
		{
			dim_counter--;
		}
		

		if (dim_counter == mx.dim)//ά�ȼ�������[����3�Σ�˵���������ά�ˣ���һά��������
		{
			for (int i1 = 0; i1 < mx.bound_length - 1; i1++)
			{
				if (mx.elemtotal >= elem_max_num)
				{
					elem_max_num += ADDLENGTH;
					mx.base = (ElemType *)realloc(mx.base, sizeof(ElemType)*elem_max_num);
					if (!mx.base)
						exit(ERROR);
				}
				scanf("%d,", &number);
				mx.base[mx.elemtotal++] = number;

			}
			if (mx.elemtotal >= elem_max_num)
			{
				elem_max_num += ADDLENGTH;
				mx.base = (ElemType *)realloc(mx.base, sizeof(ElemType)*elem_max_num);
				if (!mx.base)
					exit(ERROR);
			}
			scanf("%d", &number);
			mx.base[mx.elemtotal++] = number;


		}


		ch = getchar();
	}
	
	if (mx.elemtotal >= elem_max_num)
	{
		elem_max_num += ADDLENGTH;
		mx.base = (ElemType *)realloc(mx.base, sizeof(ElemType)*elem_max_num);
		if (!mx.base)
			exit(ERROR);
	}
	mx.base[mx.elemtotal] = '\0';





	return OK;
}


void print_element(int ex, int base)//��ӡÿһ����ʽ
{
	if (ex != 1 && ex != 0)
	{
		printf("x%dE%d", base, ex);
	}
	else if (ex == 1)
	{
		printf("x%d", base);
	}

	return;
}

Status printa()
{
	int index;
	if (mx.dim == 2)//2Ԫ
	{
		int ex1, ex2;
		for (ex1 = mx.bound_length - 1; ex1 >= 0; ex1--)
		{
			for (ex2 = mx.bound_length - 1; ex2 >= 0; ex2--)
			{
				index = ex1 * mx.bound_length + ex2;
				if (mx.base[index] == 0)
				{
					continue;
				}
				else if (mx.base[index] != 1)
				{
					printf("%d", mx.base[index]);
				}
				
				print_element(ex1, 1);
				print_element(ex2, 2);

				if (ex1 != 0 || ex2 != 0)//�������һ��ʹ�ӡ���Ӻ�
				{
					printf("+");
				}
				else//���һ��
				{
					if (mx.base[0] == 1)//������Ϊ1��ʱ��򲻳���
						printf("1");
				}
			}
		}
	}

	else if (mx.dim == 3)//3Ԫ
	{
		int ex1, ex2, ex3;
		for (ex1 = mx.bound_length - 1; ex1 >= 0; ex1--)
		{
			for (ex2 = mx.bound_length - 1; ex2 >= 0; ex2--)
			{
				for (ex3 = mx.bound_length - 1; ex3 >= 0; ex3--)
				{
					index = ex1 * mx.bound_length*mx.bound_length + ex2 * mx.bound_length + ex3;
					if (mx.base[index] == 0)
					{
						continue;
					}
					else if (mx.base[index] != 1)
					{
						printf("%d", mx.base[index]);
					}

					print_element(ex1, 1);
					print_element(ex2, 2);
					print_element(ex3, 3);
					if (ex1 != 0 || ex2 != 0 || ex3 != 0)//�������һ��ʹ�ӡ���Ӻ�
					{
						printf("+");
					}
					else//���һ��
					{
						if (mx.base[0] == 1)//������Ϊ1��ʱ��򲻳���
							printf("1");
					}
				}
			}
		}
	}


	else if (mx.dim == 1)//1Ԫ
	{
		int ex1;
		for (ex1 = mx.bound_length - 1; ex1 >= 0; ex1--)
		{
			
			index = ex1;
			if (mx.base[index] == 0)
			{
				continue;
			}
			else if (mx.base[index] != 1)
			{
				printf("%d", mx.base[index]);
			}

			print_element(ex1, 1);
			
			if (ex1 != 0 )//�������һ��ʹ�ӡ���Ӻ�
			{
				printf("+");
			}
			else//���һ��
			{
				if (mx.base[0] == 1)//������Ϊ1��ʱ��򲻳���
					printf("1");
			}
				
			
		}
	}


	else if (mx.dim == 3)//3Ԫ
	{
		int ex1, ex2, ex3;
		for (ex1 = mx.bound_length - 1; ex1 >= 0; ex1--)
		{
			for (ex2 = mx.bound_length - 1; ex2 >= 0; ex2--)
			{
				for (ex3 = mx.bound_length - 1; ex3 >= 0; ex3--)
				{
					index = ex1 * mx.bound_length*mx.bound_length + ex2 * mx.bound_length + ex3;
					if (mx.base[index] == 0)
					{
						continue;
					}
					else if (mx.base[index] != 1)
					{
						printf("%d", mx.base[index]);
					}

					print_element(ex1, 1);
					print_element(ex2, 2);
					print_element(ex3, 3);
					if (ex1 != 0 || ex2 != 0 || ex3 != 0)//�������һ��ʹ�ӡ���Ӻ�
					{
						printf("+");
					}
					else//���һ��
					{
						if (mx.base[0] == 1)//������Ϊ1��ʱ��򲻳���
							printf("1");
					}
				}
			}
		}
	}


	else if (mx.dim == 4)//4Ԫ
	{
		int ex1, ex2, ex3, ex4;
		for (ex1 = mx.bound_length - 1; ex1 >= 0; ex1--)
		{
			for (ex2 = mx.bound_length - 1; ex2 >= 0; ex2--)
			{
				for (ex3 = mx.bound_length - 1; ex3 >= 0; ex3--)
				{
					for (ex4 = mx.bound_length - 1; ex4 >= 0; ex4--)
					{
						index = ex1 * mx.bound_length*mx.bound_length*mx.bound_length + ex2 * mx.bound_length*mx.bound_length + ex3 * mx.bound_length + ex4;
						if (mx.base[index] == 0)
						{
							continue;
						}
						else if (mx.base[index] != 1)
						{
							printf("%d", mx.base[index]);
						}

						print_element(ex1, 1);
						print_element(ex2, 2);
						print_element(ex3, 3);
						print_element(ex4, 4);
						if (ex1 != 0 || ex2 != 0 || ex3 != 0 || ex4 != 0)//�������һ��ʹ�ӡ���Ӻ�
						{
							printf("+");
						}
						else//���һ��
						{
							if (mx.base[0] == 1)//������Ϊ1��ʱ��򲻳���
								printf("1");
						}
					}
				}
			}
		}
	}

	return OK;
}

