#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10
#define NUMBER 1
#define CHARACTER 2

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
	int atom_type;
}GLNode, *GList;

char emp[] = "()";
int deletechar;
int visit_array[MAXLENGTH] = { 0 };
int visit_counter = 0;
int string_tag[MAXLENGTH] = { 0 };

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
	int index3 = 0;
	int index4 = 0;
	int length_3 = 0;
	int single_ch;
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
		//还得给str加一层括号
		//str加括号
		backup_str[0] = '(';
		for (index2 = 0; str[index2] != '\0'; index2++)
		{
			backup_str[index2 + 1] = str[index2];
		}
		backup_str[index2 + 1] = ')';
		backup_str[index2 + 2] = '\0';
		strcpy(str, backup_str);
	}
	else//此时只有一对括号，没法分离，例如str = (a,b,c,d)这样
	{
		//strcpy(hstr, str);
		if (strlen(str) == 1)
		{
			/*
			single_ch = str[0];
			str[0] = '(';
			str[1] = single_ch;
			str[2] = ')';
			str[3] = '\0';
			*/
			strcpy(hstr, str);
			str[0] = '(';
			str[1] = ')';
			str[2] = '\0';
			
		}
		else if (str[1] != '(')
		{
			hstr[0] = str[1];
			hstr[1] = '\0';
			length_3 = 1;
		}
		else
		{
			for (index3 = 1; str[index3] != ')'; index3++)
			{
				hstr[index3 - 1] = str[index3];
				length_3++;
			}
			hstr[length_3] = ')';
			hstr[length_3 + 1] = '\0';
			length_3++;
		}

		int length_of_str = strlen(str);
		if (length_of_str > 3)
		{
			if (length_3 == 1) 
			{
				for (index2 = 3; index2 < length_of_str; index2++)
				{
					str[index2 - 2] = str[index2];
				}
				str[length_of_str - 2] = '\0';
			}
			else
			{
				str[index4++] = '(';
				if (str[index3 + 1] == ',')
					index2 = index3 + 2;
				else
					index2 = index3 + 1;
				for (; index2 < length_of_str; index2++)
				{
					str[index4++] = str[index2];
				}
				str[index4] = '\0';
			}
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
//	int nextch;//ch的后一位
	int index = 0;
	int depth = 0;//记录当前深度
	GList p, q;
	char sub[MAXLENGTH], hsub[MAXLENGTH];
	

	ch = string[index];

	if (strcmp(string, emp) == 0)
		*L = NULL;
	else
	{
		if (!((*L) = (GList)malloc(sizeof(GLNode))))
			return ERROR;
		if (strlen(string) == 1)//单原子广义表
		{
			(*L)->tag = atom;
			(*L)->atom = string[0];
		}
		else
		{
			(*L)->tag = list;
			p = (*L);
			substring(sub, string, 1, strlen(string) - 2);//脱括号
			do
			{
				sever(sub, hsub);
				CreateGList(&(p->ptr.hp), hsub);
				q = p;
				if (strcmp(sub, emp))//表尾不空
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
		if (ch == '(')//左括号，说明这是一个表节点
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
		else//其余元素
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
	int number_tmp1;
	int number_tmp2;
	deletechar = getchar();//待删除的元素
	ch = getchar();//除掉分号

	ch = getchar();
	while (ch != EOF && ch != '\0' && ch != '\n')
	{
		string[index++] = ch;
		ch = getchar();
		if (string[index - 1] >= '0' && string[index - 1] <= '9')
		{
			if (ch >= '0' && ch <= '9')//控制台输入了一个2位数
			{
				number_tmp1 = (int)(string[index - 1] - '0');
				number_tmp2 = (int)(ch - '0');
				number_tmp1 = number_tmp1 * 10 + number_tmp2;
				string[index - 1] = (char)number_tmp1;
				string_tag[index - 1] = NUMBER;
				ch = getchar();
			}
			else
			{
				string_tag[index - 1] = CHARACTER;
			}
		}
		else
		{
			string_tag[index - 1] = CHARACTER;
		}
		
	}

	string[index] = '\0';
	return;
}

void visit_list(GList L)
{
	
	if (L)
	{
		if (L->tag == atom)
		{
			if (L->atom != deletechar)
				visit_array[visit_counter++] = L->atom;
		}
		else
		{
			if(L->ptr.hp->tag == list)
				visit_array[visit_counter++] = '(';
			visit_list(L->ptr.hp);
			
			if(L->ptr.tp == NULL)
				visit_array[visit_counter++] = ')';
			visit_list(L->ptr.tp);
			
		}
	}
	
	return;
}

int main()
{
	GList L ;
	char string[MAXLENGTH];


	getstr(string);
	if(CreateGList(&L, string) == ERROR)
		exit(1);
	visit_array[visit_counter++] = '(';
	visit_list(L);
	for (int i = 0; i < visit_counter; i++)
	{
		if (visit_array[i] >= 'a' && visit_array[i] <= 'z')
			printf("%c", visit_array[i]);
		else if(visit_array[i] >= 'A' && visit_array[i] <= 'Z')
			printf("%c", visit_array[i]);
		else if(visit_array[i] >= '0' && visit_array[i] <= '9')
			printf("%c", visit_array[i]);
		else if(visit_array[i] == '(' || visit_array[i] == ')')
			printf("%c", visit_array[i]);
		else
			printf("%d", visit_array[i]);
		if (visit_array[i] != '(' && visit_array[i] != ')')
		{
			if (visit_array[i + 1] != '(' && visit_array[i + 1] != ')')
			{
				if (i < visit_counter - 1)
					printf(",");
			}
			else if (visit_array[i + 1] == '(')
				if (i < visit_counter - 1)
					printf(",");

		}
		else if (visit_array[i] == ')')
		{
			if (visit_array[i + 1] == '(')
			{
				if (i < visit_counter - 1)
					printf(",");
			}
			else if (visit_array[i + 1] != '(' && visit_array[i + 1] != ')')
				if (i < visit_counter - 1)
					printf(",");
		}
		
	}
	
	return 0;
}