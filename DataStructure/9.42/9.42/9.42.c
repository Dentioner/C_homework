#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define LEAF 0
#define BRANCH 1

typedef struct TrieNode
{
	int kind;
	union 
	{
		struct
		{
			int key;
			int *infoptr;
		}leaf;
		struct 
		{
			struct TrieNode *ptr[27];//ָ����0Ϊ$�ţ�1Ϊ��ĸA
			int num;//ָʾ�ýڵ��зǿ�ָ����ĸ���
		}branch;
	};
}TrieNode, *TrieTree;

TrieTree t;
int depth;

void create_tree()
{
	int ch;
	int char_index;
	TrieTree p;
	
	scanf("%d", &depth);
	ch = getchar(); // ';'

	t = (TrieTree)malloc(sizeof(TrieNode));
	t->kind = BRANCH;
	t->branch.num = 0;
	for (int i = 0; i < 27; i++)//��ʼ��
		t->branch.ptr[i] == NULL;

	p = t;
	

	ch = getchar();//����ĸ
	while (ch != ';')
	{
		char_index = ch - 'a' + 1;


		ch = getchar();
	}
}