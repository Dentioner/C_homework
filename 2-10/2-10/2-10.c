#include <stdio.h>
#define length 100
int lower(int c);

int main()
{
	int n = 0;
	char str1[length] = "JOJO'S BIZZARE ADVENTURE STARDUST CRUSADERS.";
	char str2[length] = { '\0' };
	printf("str1: %s \nstr2: %s \nlowering...\n", str1, str2);

	for (n = 0; n < length; n++)
	{
		str2[n] = lower(str1[n]);
	}
	printf("str1: %s \nstr2: %s \n", str1, str2);

	return 0;
}


int lower(int c)
{
	return ('A' <= c && c <= 'Z') ? (c + 'a' - 'A') : c;
}