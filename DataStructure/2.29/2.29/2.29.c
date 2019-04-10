#pragma warning(disable:4996);
#include<stdio.h>
#include<stdlib.h>

typedef struct linksheet
{
	int ch;
	struct linksheet* prior;
	struct linksheet* next;
}node;

void delete_node(node* pr);


node* ha;
node* hb;
node* hc;

int main()
{
	ha = (node*)malloc(sizeof(node));
	hb = (node*)malloc(sizeof(node));
	hc = (node*)malloc(sizeof(node));
	ha->prior = NULL;
	hb->prior = NULL;
	hc->prior = NULL;
	node* now = ha;
	node* pra, *prb, *prc, *waste;
	int cha, chb, chc;
	int findit;
	while ((cha = getchar())!=';')
	{
		if (cha != ',')
		{
			now->next = (node*)malloc(sizeof(node));
			now->next->prior = now;
			now->ch = cha;
			now = now->next;
		}
	}
	now->prior->next = NULL;
	free(now);
	now = hb;
	while ((cha = getchar()) != ';')
	{
		if (cha != ',')
		{
			
			now->next = (node*)malloc(sizeof(node));
			now->next->prior = now;
			now->ch = cha;
			now = now->next;
		}
	}
	now->prior->next = NULL;
	free(now);
	now = hc;
	while ((cha = getchar()) != '\n')
	{
		if (cha != ',')
		{
			now->next = (node*)malloc(sizeof(node));
			now->next->prior = now;
			
			now->ch = cha;
			now = now->next;
		}
	}
	now->next = NULL;
	now->prior->next = NULL;
	free(now);

	pra = ha;
	prb = hb;
	prc = hc;
	while (pra!=NULL)
	{
		cha = pra->ch;
		while (prb != NULL)
		{
			findit = 0;
			chb = prb->ch;
			if (chb == cha)
			{
				while (prc != NULL)
				{
					chc = prc->ch;
					if (chc == chb)
					{
						waste = pra;
						pra = ha;
						delete_node(waste);
						
						findit = 1;
						break;
					}
					else
					{
						prc = prc->next;
					}

				}
				
				if (findit)
				{
					pra = ha;
					prb = hb;
					
					
					break;
				}
			}
			prb = prb->next;
			prc = hc;
		}



		pra = pra->next;
		prb = hb;
		prc = hc;
	}
	pra = ha;

	while (pra->next != NULL)
	{
		printf("%c,", pra->ch);
		pra = pra->next;
	}
	printf("%c", pra->ch);
	system("pause");
	return 0;
}


void delete_node(node* pr)
{
	if (pr == ha)
		ha = ha->next;
		
	else
	{
		pr->prior->next = pr->next;
		if (pr->next != NULL)
			pr->next->prior = pr->prior;
	}
	free(pr);
}