#include<stdio.h>
#include<stdlib.h>

#define MAXLENGTH 50

int findit = 0;
int T[MAXLENGTH] = {0};
int n;

void create_array(int a[])
{
    int ch;
    int number;
    int index = 0;
    scanf("%d", &number);
    ch = getchar();
    while (ch != EOF && ch != ';' && ch != '\n')
    {
        a[index ++ ]= number;
        scanf("%d", &number);
        ch = getchar();
    }
    a[index ++ ]= number;
    a[index] = '\0';
    n = index;
    return;
}

void creatT(int A[])
{
    int son, parent;
    for (parent = 0; parent < n; parent ++)
    {
        son = A[parent];
        T[son] = parent; 
    }

    return;
}

void find(int p,int v, int L[], int R[])
{

    if (p == v)
    {
        findit = 1;
        return;
    }
    else if (p!= 0)
    {
        p = T[p];
        find(p, v, L, R);
    }
    else
    {
        return;
    }
    


}

int main()
{
    int L[MAXLENGTH], R[MAXLENGTH];
    int u, v;
    
    create_array(L);
    create_array(R);
    scanf("%d;%d", &u, &v);
    creatT(L);
    creatT(R);
    T[0] = 0;

    find(u, v, L, R);
    printf("%d", findit);


    system("pause");
    return 0;
}