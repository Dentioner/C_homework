#include <stdio.h>
#define MAXLINE 1000 /* 最大的输入行长度 */

int getline(char line[], int max);//声明getline函数
int strindex(char source[], char searchfor[]);//声明strindex函数

char pattern[] = "1234"; /* 待查找的内容*/

/* 找出所有与待查找内容相同的行*/
main()
{
	char line[MAXLINE];//定义一个1000单位长度的char型数组
	int found = 0;//定义变量found

	while (getline(line, MAXLINE) > 0)//如果输入的字符串长度大于0
	{
		if (strindex(line, pattern) >= 0)//由于没找到返回值是-1，因此只要返回值比-1大，也就是返回值不比0小，就说明找到了1234
		{
			printf("%s", line);//打印待搜索的字符串line
			found++;//每一次循环让found+1
		}
	}
	return found;//最后返回的是整个程序执行过程中查询到相同字符串的次数
}

/* getline: 将行内容保存到s中，并返回该行的长度 */
int getline(char s[], int lim)//函数getline的定义
{
	int c, i;
	i = 0;

	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		//出现这三种情况之一时跳出循环：
		//1.输入字符数量超过MAXLINE规定
		//2.输入EOF时
		//3.输入回车时
		s[i++] = c;//将新输入的字符添加到输入变量字符串s中

	if (c == '\n')//如果跳出循环的时候输入的是回车，那么将回车也加进去
		s[i++] = c;

	s[i] = '\0';//结束之后将字符串末尾添加转义字符\0，以表示字符串的末尾处

	return i;//返回输入的字符串的长度
}

/* strindex: 返回t在s中的位置，如果没有找到t，返回-1 */
int strindex(char s[], char t[])//strindex的定义
{
	int i, j, k;
	for (i = 0; s[i] != '\0'; i++)//读取输入的s数组，读到数组末尾为止
	{//第一层循环的每一次循环，代表着从s[i]往后开始4个长度搜索1234的一个过程
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;//第二层循环，空语句
		//跳出第二层循环的一种情况是，读取到了待查找的字符串t（即“1234”）的末尾的时候；
		//跳出第二层循环的另一种情况是，在字符串s中没有读取到待查找的字符串t的某个相同的字符的时候
		//第二种情况的另一个解释是，第二层循环是在查找到了字符1之后，再看下一个是不是2，如果是2，再看下一个是不是3，如果是3，再看下一个是不是4
		//只要没有查到连续的1234四个字符，就会跳出第二层循环
		if (k > 0 && t[k] == '\0')//如果读取到了t的末尾的时候，此时说明t中所有的字符都找到了对应的位置，也就是检索到了1234这个字段
			return i;//返回此次循环的起始搜索处的位置i
	}
	return -1;//s搜完了还没有返回，说明在s中没有找到1234这个字符串
}