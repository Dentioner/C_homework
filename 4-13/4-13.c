#include<string.h>
#include<stdio.h>
#include<stdlib.h>
void reverse2(char s[], int i, int j)
{
    int c;
    if (i < j)
    {
        c = s[i];
		s[i] = s[j];
		s[j] = c;
        reverse2(s, i + 1, j - 1);
    }
}
int main()
{
	char s[11] = {"abcdefghij"};
    printf("??????\n");
    printf(s);
    printf("\n");
	reverse2(s, 0, strlen(s) - 1);
    printf("?????\n");
    printf(s);
    printf("\n");
    system("pause");
	return 0;

}
