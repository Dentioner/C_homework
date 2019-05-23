#include<stdio.h>
#include<stdlib.h>

#define MAXLENGTH 50
#define ADDLENGTH 10

typedef enum {OK, ERROR} status;
typedef int ElemType ;

typedef struct matrix
{
    ElemType * base;
    int raw;
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
        printf("%d", ap.base[0]);
        for (int i = 1; i < ap.num; i++)
            printf(",%d", ap.base[i]);
    }
    
    
    system("pause");
    return 0;
}




status InitMatrix()
{
    int ch; 
    int elem_max_num = MAXLENGTH;
    int index = 0;
    m.base = (ElemType *)malloc(sizeof(ElemType)*elem_max_num);
    if (!m.base)
        exit(ERROR);
    ch = getchar();
    while(ch != EOF && ch != '\n')
    {
        while (ch != ';')
        {
            if (ch != ',')
            {
                if (m.elemtotal >= elem_max_num)
                {
                    elem_max_num += ADDLENGTH;
                    m.base = (ElemType *)realloc(m.base, sizeof(ElemType)*elem_max_num);
                    if (!m.base)
                        exit(ERROR);
                }
                m.base[m.raw * m.column + index] = ch;
                index ++;
            }
            ch = getchar();
        }
        if (m.raw == 0)
            m.column = index;
        else if (m.column != index)
            exit(ERROR);
        m.raw ++;
        index = 0;
        ch = getchar();
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
    for (i = 0; i < m.raw; i++)
    {
        bestj = 0;
        for (j = 0; j <m.column; j ++)
        {
            if (m.base[i*m.column + bestj] > m.base[i*m.column + j])
                bestj = j;
        }

        for (k = 0; k < m.raw; k++)
        {
            if (m.base[i*m.column + bestj] < m.base[k*m.column + bestj])
                break;
        }

        if (k == m.raw)
        {
            ap.base[ap.num ++] = m.base[i*m.column + bestj];
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
    for (i=0; i<ap.num-1; i++) /* 外循环为排序趟数，len个数进行len-1趟 */
        for (j=0; j<ap.num-1-i; j++) 
        { /* 内循环为每趟比较的次数，第i趟比较len-i次 */
            if (ap.base[j] > ap.base[j+1])
            { /* 相邻元素比较，若逆序则交换（升序为左大于右，降序反之） */
                temp = ap.base[j];
                ap.base[j] = ap.base[j+1];
                ap.base[j+1] = temp;
            }
        }

}


