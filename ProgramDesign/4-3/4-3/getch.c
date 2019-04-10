#include<stdio.h>

#define BUFSIZE 100

char buf[BUFSIZE]; /*用于unget函数的缓冲区*/
int bufp = 0;/*buf中下一个自由位置*/

int getch(void)/*取一个字符（可能是推回的字符）*/
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)/*把字符推回到输入中*/
{
	if (bufp >= BUFSIZE)
		printf("ungetch:too many characters\n");
	else
		buf[bufp++] = c;
}
