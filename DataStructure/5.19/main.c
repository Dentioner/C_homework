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

status InitMatrix();
status an_search();
void an_sort();

int main()
{
    if(InitMatrix())
        exit(ERROR);
    if(an_search())
        exit(ERROR);
    if (ap.num == 0)
        printf("null");
    else
    {
		;//printf("%d", ap.base[0]);
		for (int i = 1; i < ap.num; i++)
			;//printf(",%d", ap.base[i]);
    }
    
    
   // system("pause");
    return 0;
}




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

status an_search()
{
    int i, j, k;
    int bestj;
    ap.base = (ElemType*)malloc(sizeof(ElemType)*m.elemtotal);
    if (!ap.base)
        exit(ERROR);
    for (i = 0; i < m.row; i++)
    {
        bestj = 0;
        for (j = 0; j <m.column; j ++)
        {
            if (m.base[i*m.column + bestj] > m.base[i*m.column + j])
                bestj = j;
        }

		for (j = 0; j < m.column; j++)
		{
			if (m.base[i*m.column + bestj] == m.base[i*m.column + j])
			{
				for (k = 0; k < m.row; k++)
				{
					if (m.base[i*m.column + j] < m.base[k*m.column + j])
						break;
				}
			}

			if (k == m.row)
			{
				ap.base[ap.num++] = m.base[i*m.column + j];
			}
			k = 0;
		}
    }

    ap.base[ap.num] = '\0';

    an_sort();

    return OK;
}

void an_sort()
{
    int i,j;
    ElemType temp;
    for (i=0; i<ap.num-1; i++)
        for (j=0; j<ap.num-1-i; j++) 
        { 
            if (ap.base[j] > ap.base[j+1])
            { 
                temp = ap.base[j];
                ap.base[j] = ap.base[j+1];
                ap.base[j+1] = temp;
            }
        }

}


