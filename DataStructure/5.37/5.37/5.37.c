#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10

typedef enum { OK, ERROR } Status;
typedef int ElemType;

typedef enum{atom, list} elemtag;
typedef struct GLNode
{
	elemtag tag;
	union 
	{
		ElemType atom;
		struct 
		{
			struct GLNode *hp, *tp;
		}ptr;
	};
}GLNode, *GList;

char emp[] = "()";
int deletechar;


Status substring(char sub[], char str[], int pos, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		sub[i] = str[pos + i];
	}
	sub[i] = '\0';

	return OK;
}

void sever(char str[], char hstr[])
{
	int n = strlen(str);
	int i = -1;
	int k = 0;
	char ch;
	int index2 = 0;
	char backup_str[MAXLENGTH] = { 0 };
	do
	{
		i++;
		ch = str[i];
		if (ch == '(')
			k++;
		else if (ch == ')')
			k--;

	} while (i < n && (ch != ',' || k != 0));

	if (i < n)
	{
		substring(hstr, str, 0, i );
		substring(str, str, i + 1, n - i);
		//���ø�str��һ������
		//str������
		backup_str[0] = '(';
		for (index2 = 0; str[index2] != '\0'; index2++)
		{
			backup_str[index2 + 1] = str[index2];
		}
		backup_str[index2 + 1] = ')';
		backup_str[index2 + 2] = '\0';
		strcpy(str, backup_str);
	}
	else//��ʱֻ��һ�����ţ�û�����룬����str = (a,b,c,d)����
	{
		//strcpy(hstr, str);
		hstr[0] = str[1];
		hstr[1] = '\0';
		
		int length_of_str = strlen(str);
		if (length_of_str > 3)
		{
			for (index2 = 3; index2 < length_of_str; index2++)
			{
				str[index2 - 2] = str[index2];
			}
			str[length_of_str - 2] = '\0';
		}
		else
		{
			str[1] = ')';
			str[2] = '\0';
		}



	}

	

	return;
}

Status CreateGList(GList *L, char string[])
{
	int ch;
//	int nextch;//ch�ĺ�һλ
	int index = 0;
	int depth = 0;//��¼��ǰ���
	GList p, q;
	char sub[MAXLENGTH], hsub[MAXLENGTH];
	

	ch = string[index];

	if (strcmp(string, emp) == 0)
		*L = NULL;
	else
	{
		if (!((*L) = (GList)malloc(sizeof(GLNode))))
			return ERROR;
		if (strlen(string) == 1)//��ԭ�ӹ����
		{
			(*L)->tag = atom;
			(*L)->atom = string[0];
		}
		else
		{
			(*L)->tag = list;
			p = (*L);
			substring(sub, string, 1, strlen(string) - 2);//������
			do
			{
				sever(sub, hsub);
				CreateGList(&(p->ptr.hp), hsub);
				q = p;
				if (strcmp(sub, emp))//��β����
				{
					if (!(p = (GList)malloc(sizeof(GLNode))))
						return ERROR;
					p->tag = list;
					q->ptr.tp = p;
				}

			} while (strcmp(sub, emp));
			q->ptr.tp = NULL;
		}//else
	}//else



	/*
	while (ch !=  '\0')
	{
		if (!(p = (GList)malloc(sizeof(GLNode))))
			return ERROR;
		if (ch == '(')//�����ţ�˵������һ����ڵ�
		{
			depth++;
			p->tag = list;
		}
		else if (ch == ',')
		{

		}
		else if (ch == ')')
		{
			depth--;
		}
		else//����Ԫ��
		{

		}

		
	}
	*/
	return OK;
}

void getstr(char string[])
{
	char ch;
	int index = 0;
	deletechar = getchar();//��ɾ����Ԫ��
	ch = getchar();//�����ֺ�

	ch = getchar();
	while (ch != EOF && ch != '\0' && ch != '\n')
	{
		string[index++] = ch;
		ch = getchar();
	}

	string[index] = '\0';
	return;
}

int main()
{
	GList L ;
	char string[MAXLENGTH];


	getstr(string);
	if(CreateGList(&L, string) == ERROR)
		exit(1);

	system("pause");
	return 0;
}