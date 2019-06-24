#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10

typedef enum {OK, ERROR} status;
typedef int ElemType ;

typedef struct matrix
{
    ElemType * base;
    int row;
    int column;
    int elemtotal;    
}mx;

mx m;

typedef struct 
{
    ElemType * base;
    int num;
}an_point;

an_point ap;
ElemType min[MAXLENGTH] = {0};
ElemType max[MAXLENGTH] = {0};


status InitMatrix()
{
    int ch, number; 
    int elem_max_num = MAXLENGTH;
    int index = 0;
    m.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
    if (!m.base)
        exit(ERROR);
	if (!scanf("%d", &number))
		exit(ERROR);
    ch = getchar();
	while (ch != EOF && ch != '\n')
	{
		while (ch != ';'&& ch != EOF && ch != '\n')
		{

			if (m.elemtotal >= elem_max_num)
			{
				elem_max_num += ADDLENGTH;
				m.base = (ElemType *)realloc(m.base, sizeof(ElemType)*elem_max_num);
				if (!m.base)
					exit(ERROR);
			}
			m.base[m.row * m.column + index] = number;
			index++;

			if (!scanf("%d", &number))
				exit(ERROR);
			ch = getchar();
		}
		if (ch != ';' || ch != EOF || ch != '\n')
		{
			if (m.elemtotal >= elem_max_num)
			{
				elem_max_num += ADDLENGTH;
				m.base = (ElemType *)realloc(m.base, sizeof(ElemType)*elem_max_num);
				if (!m.base)
					exit(ERROR);
			}
			m.base[m.row * m.column + index] = number;
			index++;
		}
		if (m.row == 0)
			m.column = index;
		else if (m.column != index)
			exit(ERROR);
		m.row++;
		index = 0;
		if (ch != EOF && ch != '\n')
		{
			if (!scanf("%d", &number))
				exit(ERROR);
			ch = getchar();

		}
	}
    return OK;
}

void create_min_row()
{
	int index1, index2;
	int min_ele;
	for(index1 = 0; index1 < m.row; index1++)
	{
		min_ele = m.base[index1 * m.column];
		for(index2 = 0; index2 < m.column; index2++)
		{
			if (m.base[index1 * m.column + index2] < min_ele)
				min_ele = m.base[index1 * m.column + index2];
		}
		min[index1] = min_ele;
	}
}

void create_max_column()
{
	int index1, index2;
	int max_ele;
	for(index1 = 0; index1 < m.column; index1++)
	{
		max_ele = m.base[index1 * m.column];
		for(index2 = 0; index2 < m.row; index2++)
		{
			if (m.base[index1 * m.column + index2] > max_ele)
				max_ele = m.base[index1 * m.column + index2];
		}
		max[index1] = max_ele;
	}
}


int main()
{
    if(InitMatrix())
        exit(ERROR);
    //if(an_search())
        //exit(ERROR);
	create_min_row();
	create_max_column();
    
    
   // system("pause");
    return 0;
}




