#include<stdio.h>
#include<stdlib.h>


#define MAXLENGTH 20
#define ADDLENGTH 10

int addlen(int * x, int len)
{
	x = (int*)realloc(x, (len + ADDLENGTH) * sizeof(int));
	if (!x)
		printf("ERROR");
	len += ADDLENGTH;
	return len;
}


int main()
{
	int ch;
	int* s, *t;
	int slen, tlen;
	int sindex = 0;
	int tindex = 0;
	int answer = 0;
	s = (int*)malloc(MAXLENGTH * sizeof(int));
	t = (int*)malloc(MAXLENGTH * sizeof(int));
	slen = MAXLENGTH;
	tlen = MAXLENGTH;


	while ((ch = getchar())!=' ')
	{
		s[sindex] = ch;
		sindex++;
		if (sindex >= slen)
			slen = addlen(s, slen);
	}
	


	while ((ch = getchar()) != '\n')
	{
		t[tindex] = ch;
		tindex++;
		if (tindex >= tlen)
			tlen = addlen(t, tlen);
	}

	for (int i = 0; i < tindex && i < sindex; i++)
	{
		if (s[i] > t[i])
		{
			answer = 1;
			break;
		}
		else if (s[i] < t[i])
		{
			answer = -1;
			break;
		}
	}
	if (answer == 0)
	{
		if (sindex > tindex)
			answer = 1;
		else if (sindex < tindex)
			answer = -1;
	}

	printf("%d", answer);

	system("pause");
	return 0;
}


