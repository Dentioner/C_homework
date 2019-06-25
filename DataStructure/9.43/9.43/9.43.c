#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define LEAF 0
#define BRANCH 1
#define MAXLENGTH 20

typedef struct TrieNode
{
	int kind;
	union
	{
		struct
		{
			int key;
			int *infoptr;
		}leaf;//Ҷ��
		struct
		{
			struct TrieNode *ptr[27];//ָ����0Ϊ$�ű�ʾû�к����ĸ�ˣ�1Ϊ��ĸA
			int num;//ָʾ�ýڵ��зǿ�ָ����ĸ���
		}branch;//��֧
	};
}TrieNode, *TrieTree;

TrieTree t;
int depth;
int visit_array[MAXLENGTH];
int visit_counter = 0;

void insert(char character[], int cha_len)
{
	TrieTree p;
	int char_index;
	int current_depth;
	p = t;
	current_depth = 0;
	t->branch.num++;
	do
	{
		char_index = character[current_depth] - 'a' + 1;
		if (p->branch.ptr[char_index] == NULL)
		{
			p->branch.ptr[char_index] = (TrieTree)malloc(sizeof(TrieNode));
			for (int i = 0; i < 27; i++)//��ʼ��
				p->branch.ptr[char_index]->branch.ptr[i] = NULL;
			p->branch.ptr[char_index]->branch.num = 0;
			p->branch.ptr[char_index]->kind = BRANCH;
		}
		p->branch.ptr[char_index]->branch.num++;
		p = p->branch.ptr[char_index];
		current_depth++;
	} while (current_depth < depth && current_depth < cha_len);



	return;
}

void delete_key(char character[], int cha_len)
{
	TrieTree p;
	int char_index;
	int current_depth;
	p = t;
	current_depth = 0;
	t->branch.num--;
	do
	{
		char_index = character[current_depth] - 'a' + 1;
		if (p->branch.ptr[char_index] == NULL)//˵��û�йؼ���
		{
			break;
		}
		p->branch.ptr[char_index]->branch.num--;
		p = p->branch.ptr[char_index];
		current_depth++;
	} while (current_depth < depth && current_depth < cha_len);


	return;
}

void create_tree()
{
	int ch;
	int char_index;
	TrieTree p;
	char character[MAXLENGTH];
	int char_len = 0;
	scanf("%d", &depth);
	ch = getchar(); // ';'

	t = (TrieTree)malloc(sizeof(TrieNode));
	t->kind = BRANCH;
	t->branch.num = 0;
	for (int i = 0; i < 27; i++)//��ʼ��
		t->branch.ptr[i] = NULL;

	p = t;


	ch = getchar();//����ĸ
	while (ch != '\n' && ch != EOF)//����������ĸ���붼������
	{

		char_len = 0;
		while (ch != ';'  && ch != '\n'&& ch != EOF)//ÿһ�����ʵ�ѭ��
		{

			character[char_len++] = ch;


			ch = getchar();
		}
		character[char_len] = '\0';
		if (ch == ';')
			insert(character, char_len);
		else
			delete_key(character, char_len);
		if (ch != '\n')
			ch = getchar();
	}

	return;
}

void Visit(int e)
{
	/*
	if (visit_counter < length - 1)
		printf("%d,", e);
	else
		printf("%d", e);
	visit_counter++;
	*/
	visit_array[visit_counter] = e;
	visit_array[visit_counter + 1] = '\0';
	visit_counter++;
	return;
}

void PreorderTraverse(TrieTree T, void(*Visit)(int e))
{
	int i;
	if (T)
	{
		if (T != t)
			Visit(T->branch.num);
		for (i = 0; i < 27; i++)
		{
			if (T->branch.ptr[i] != NULL)
			{
				PreorderTraverse(T->branch.ptr[i], Visit);
			}
		}

	}
	return;
}

void visit_print()
{
	int index;
	for (index = 0; index < visit_counter - 1; index++)
		printf("%d,", visit_array[index]);
	printf("%d", visit_array[index]);
	return;
}

int main()
{
	TrieTree T;
	create_tree();
	T = t;
	PreorderTraverse(T, Visit);
	visit_print();
	return 0;
}