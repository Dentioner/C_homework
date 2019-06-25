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
		}leaf;//叶子
		struct 
		{
			struct TrieNode *ptr[27];//指针域，0为$号表示没有后继字母了，1为字母A
			int num;//指示该节点中非空指针域的个数
		}branch;//分支
	};
}TrieNode, *TrieTree;

TrieTree t;
int depth;

void insert(char character[], int cha_len)
{
	TrieTree p;
	int char_index;
	int current_depth;
	p = t;
	current_depth = 0;
	while (current_depth < depth - 1 && current_depth < cha_len)
	{
		char_index = character[current_depth] - 'a' + 1;
		if (p->branch.ptr[char_index] == NULL)
		{
			p->branch.ptr[char_index] = (TrieTree)malloc(sizeof(TrieNode));
			for (int i = 0; i < 27; i++)//初始化
				p->branch.ptr[char_index]->branch.ptr[i] = NULL;
			p->branch.ptr[char_index]->branch.num = 0;
			p->branch.ptr[char_index]->kind = BRANCH;
		}
		p->branch.ptr[char_index]->branch.num++;
		p = p->branch.ptr[char_index];
		current_depth++;
	}

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
	for (int i = 0; i < 27; i++)//初始化
		t->branch.ptr[i] = NULL;

	p = t;
	

	ch = getchar();//读字母
	while (ch != '\n' && ch != EOF)//后续所有字母输入都在这里
	{

		char_len = 0;
		while (ch != ';'  && ch != '\n'&& ch != EOF)//每一个单词的循环
		{

			character[char_len++] = ch;


			ch = getchar();
		}
		character[char_len] = '\0';
		insert(character, char_len);
		ch = getchar();
	}

	return;
}

int main()
{
	create_tree();
	return 0;
}