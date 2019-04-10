#include<stdio.h>
#include<stdlib.h>



typedef struct node{
	int ch;
	int site;
	struct node* prior;
	struct node* next;
}node;

node *s, *t, *now, *prior, *now2, *now3;

int main()
{
	s = (node*)malloc(sizeof(node));
	t = (node*)malloc(sizeof(node));
	
	int ch;
	int s_len = 0;
	int t_len = 0;
	
	int findit = 0;

	prior = s;

	while ((ch = getchar())!= ' ')
	{
		now = (node*)malloc(sizeof(node));
		now->ch = ch;
		now->site = s_len;
		s_len++;
		now->prior = prior;
		prior->next = now;
		now = now->next;
		prior = prior->next;
	}
	prior->next = NULL;

	prior = t;
	while ((ch = getchar()) != '\n')
	{
		now = (node*)malloc(sizeof(node));
		now->ch = ch;
		now->site = t_len;
		t_len++;
		now->prior = prior;
		prior->next = now;
		now = now->next;
		prior = prior->next;
	}
	prior->next = NULL;


	

	now = s->next;
	now2 = t->next;

	
	for (int j = 0; j < s_len && now != NULL; j++)
	{
		for (int i = 0; i < t_len && now2 != NULL; i++)
		{
			if (now->ch == now2->ch)//将相同的节点直接删掉
			{
				now->prior->next = now->next;
				if (now->next != NULL)
					now->next->prior = now->prior;

				//free(???)

				findit = 1;

				break;
			}

			now2 = now2->next;
		}

		if (!findit)
		{
			now3 = (node*)malloc(sizeof(node));
			now3->ch = now->ch;
		
			t_len++;
			now3->prior = prior;
			prior->next = now3;
			
			prior = prior->next;
			prior->next = NULL;
		}



		now = now->next;
		now2 = t->next;
		findit = 0;
	}


	now = s->next;
	if (now == NULL)
		printf("-1");
	else
	{
		for (int i = 0; now != NULL; i++)
		{
			printf("%c", now->ch);
			now = now->next;
		}
		printf(" ");
		now = s->next;
		for (int i = 0; now != NULL; i++)
		{
			printf("%d", now->site);
			now = now->next;
		}
	}

	system("pause");
	return 0;
}