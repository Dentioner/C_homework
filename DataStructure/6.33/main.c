#include<stdio.h>
#include<stdlib.h>

#define MAXLENGTH 50

int findit = 0;

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

    return;
}

void find(int u, int v, int L[], int R[])
{
    int p, q;
    p = L[v];
    q = R[v];
    
    if (p == u || q== u)
    {
        findit = 1;
    }
    else if (p == 0 && q == 0)
    {
        return;
    }
    find (u, p, L, R);
    find (u, q, L, R);
}

int main()
{
    int L[MAXLENGTH], R[MAXLENGTH];
    int u, v;
    
    create_array(L);
    create_array(R);
    scanf("%d;%d", &u, &v);
    find(u, v, L, R);
    printf("%d", findit);


    system("pause");
    return 0;
}