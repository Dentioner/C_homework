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
	int row;
	int column;
	int elemtotal;
}Array;

Array mx , b;

int not_equal_to_zero;

Status InitArray();
Status printmx();
Status printb();

int main()
{
	if (InitArray() == ERROR)
		exit(1);
	if (printmx() == ERROR)
		exit(1);
	if (printb() == ERROR)
		exit(1);


	system("pause");
	return 0;

}


Status InitArray()
{
	int row, column;
	int elem_max_num = MAXLENGTH;
	int ch, number;
	int dim_counter = 0;
	not_equal_to_zero = 0;
	//int number_counter = 0;
	scanf("%d;%d;", &row, &column);
	mx.row = row;
	b.row = row;
	//mx.bounds = (int*)malloc(dim * sizeof(int));
	//if (!mx.bounds)
	//	exit(1);
	mx.column = column;
	b.column = column;
	mx.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
	b.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
	mx.elemtotal = 0;
	b.elemtotal = 0;
	if (!mx.base)
		return ERROR;
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


		if (dim_counter == 2)//维度计数，当[出现2次，说明到了最低维了，这一维就是数字
		{
			for (int i1 = 0; i1 < mx.column - 1; i1++)
			{
				if (mx.elemtotal >= elem_max_num)
				{
					elem_max_num += ADDLENGTH;
					mx.base = (ElemType *)realloc(mx.base, sizeof(ElemType)*elem_max_num);
					b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

					if (!mx.base)
						return ERROR;
				}
				scanf("%d,", &number);
				mx.base[mx.elemtotal++] = number;
				if (number)
				{
					b.base[b.elemtotal++] = 1;
					not_equal_to_zero++;
				}
				else
					b.base[b.elemtotal++] = 0;
			}
			if (mx.elemtotal >= elem_max_num)
			{
				elem_max_num += ADDLENGTH;
				mx.base = (ElemType *)realloc(mx.base, sizeof(ElemType)*elem_max_num);
				b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

				if (!mx.base)
					return ERROR;
			}
			scanf("%d", &number);
			mx.base[mx.elemtotal++] = number;
			if (number)
			{
				b.base[b.elemtotal++] = 1;
				not_equal_to_zero++;
			}
			else
				b.base[b.elemtotal++] = 0;
		}


		ch = getchar();
	}

	if (mx.elemtotal >= elem_max_num)
	{
		elem_max_num += ADDLENGTH;
		mx.base = (ElemType *)realloc(mx.base, sizeof(ElemType)*elem_max_num);
		b.base = (ElemType *)realloc(b.base, sizeof(ElemType)*elem_max_num);

		if (!mx.base)
			return ERROR;
	}
	mx.base[mx.elemtotal] = '\0';
	b.base[b.elemtotal] = '\0';




	return OK;
}

Status printb()
{
	int i, j;
	int index = 0;
	printf("[");
	for (i = 0; i < b.row; i++)
	{
		printf("[");
		for (j = 0; j < b.column - 1; j++)
		{
			
			printf("%d,", b.base[index++]);
		}
		if (index != b.elemtotal-1)
			printf("%d];", b.base[index++]);
		else
			printf("%d]", b.base[index++]);
	}
	printf("]");


	return OK;
}

Status printmx()
{
	int index = 0;
	int counter = 0;
	printf("[");
	while (index < mx.elemtotal)
	{
		if (mx.base[index] != 0)
		{
			if (counter < not_equal_to_zero - 1)
				printf("%d,", mx.base[index]);
			else
				printf("%d", mx.base[index]);
		
		counter++;
		}

		
		index++;
	}
	printf("];");

	return OK;
}