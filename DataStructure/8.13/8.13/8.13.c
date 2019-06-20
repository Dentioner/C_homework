#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
//#define FootLoc(p) (p+p->size-1)



typedef struct WORD
{
	
	struct WORD *llink;//ͷ����ָ����б��ǰ����㣬�����head�õ�
	struct WORD *uplink;//β����ָ�򱾽���ͷ���������foot�õ�
	struct WORD *tail;
	int tag;//0�����У�1��ռ��
	int location;//��ʼ��ַ
	int size;
	struct WORD *rlink;//ͷ����,ָ���̽��

}word, head, foot, *space;


const int e = 16; //������С�ڵ���e��ʣ����
space pav = NULL;

space FootLoc(space p) 
{ //���䵥λΪ �ַ�/�ֽ�
	char *q; 
	space f;
	q = (char *)p;
	f = (space)(q + sizeof(char)*p->size - sizeof(foot));
	return f;
}

space AllocBoundTag(space *pav, int n)
{//���в�С��n�Ŀ��п飬�������Ӧ�Ĵ洢�飬���������׵�ַ�����򷵻�NULL
	//pav���ǿ��б�
	space p, f;
	for (p = (*pav); p && p->size < n && p->rlink != (*pav); p = p->rlink)
		;//���Ҳ�С��n�Ŀ��п�
	if (!p || p->size < n)
		return NULL;//�Ҳ����ͷ���NULL
	else
	{//��ʱp�����ҵ��Ŀ��п�
		f = FootLoc(p);//ָ��p�ĵײ�
		(*pav) = p->rlink;//pavָ��p���¼�
		if (p->size - n <= e)
		{//ʣ�����Ĵ�С������ֵ�ˣ�ȫ���׸�
			if ((*pav) == p)
				pav = NULL;//�����ÿռ���ɿձ���
			else
			{
				(*pav)->llink = p->llink;
				p->llink->rlink = (*pav);
			}
			p->tag = f->tag = 1;//�޸ķ���ڵ��ͷ���͵ײ���־
		}
		else//����������������n����
		{
			f->tag = 1;//�޸ĵײ���־
			p->size -= n;//��С��ȥn
			f = FootLoc(p);//ָ��ʣ���ĵײ�
			f->tag = 0;
			f->uplink = p;
			p = f + 1;//ָ�򱻷���Ŀ��ͷ��
			p->tag = 1;//��Ǹĳ�1����ʹ����
			p->size = n;
		}
		return p;

	}//else
	

}//AllocBoundTag

void FreeBoundTag(space *pav, int loc, int size)
{
	word *p, *q, *f, *s, *t, *q1; 
	int n;
	space index1;
	index1 = (*pav);
	while (1)
	{
		if (index1->location < loc && index1->rlink->location >loc)
			break;
		index1 = index1->rlink;
	}

	//p = (word *)(x - sizeof(word));
	p = (space)(index1 + (loc - index1->location));
	p->size = size;
	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 1) 
	{//�����ڿ��Ϊռ�ÿ飬��ʱֻҪ���򵥵Ĳ��뼴��
		p->tag = 0;
		f = FootLoc(p);
		f->uplink = p;
		f->tag = 0;
		if (!(*pav))
			(*pav) = p->llink = p->rlink = p; //��һ�����п�
		else 
		{
			q = (*pav)->llink;  p->rlink = (*pav);
			p->llink = q;
			q->rlink = p;
			(*pav)->llink = p;
			(*pav) = p;
			//pavָ����ͷŵĿ��н��
			//���ͷŵĿ��н���ǲ���pav֮ǰ������Ϊpav
			//�´η������Ȳ�ѯ�Ľ��
		}
		return;
	}

	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 1) 
	{//���ڿ���ж����ڿ�Ϊռ��, ֻҪ�ı����ڿ��п�Ľڵ㣬Ҳ���Ǻ���ߺ���
		n = p->size;
		s = (p - 1)->uplink; 
		s->size += n; //�����¿��п��С
		f = FootLoc(p); 
		//f=p+n-1;
		f->uplink = s; 
		f->tag = 0; //�����¿��п�ײ�
		return;
	}

	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 0) 
	{//����ռ�ö����ڿ���, ������ͬ��
		t = FootLoc(p) + 1; 
		//t = p + p->size;
		p->tag = 0; 
		q = t->llink;
		p->llink = q; 
		q->rlink = p; // ���ÿ��п��ǰ��
		q1 = t->rlink; 
		p->rlink = q1; 
		q1->llink = p;
		p->size += t->size; //�����¿��п��С
		FootLoc(t)->uplink = p; //�ײ�ָ��ָ���½���ͷ��
		return;
	}
	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 0) 
	{ //�����ڿ��Ϊ���п飬һ�����
		n = p->size;
		s = (p - 1)->uplink;	//sָ�����ڿ�
		t = FootLoc(p) + 1;     //t=p+p->size; 	//tָ�����ڿ�
		s->size += n + t->size; //�����½��Ĵ�С
		q = t->llink; 
		q1 = t->rlink; //�޸�����ԭ���Ĺ�ϵ
		q->rlink = q1; 
		q1->llink = q;
		FootLoc(t)->uplink = s; //�½��ײ�ָ��ָ����ͷ��
		return;
	}
}

space create_pav()
{
	space p, q;
	int tag, location, size;
	int ch;
	int counter = 0;
	pav = NULL;
	ch = getchar();
	p = q = NULL;
	
	while (ch != '\n')
	{
		scanf(" %d %d", &location, &size);
		tag = ch - '0';
		if (counter == 0)
		{
			pav = (space)malloc(sizeof(word));
			pav->tag = tag;
			pav->location = location;
			pav->size = size;
			pav->llink = NULL;
			pav->uplink = NULL;
			
			pav->tail = (space)malloc(sizeof(word));
			pav->tail->uplink = pav;
			pav->tail->location = pav->location + pav->size;
			pav->tail->tag = pav->tag;
			pav->tail->llink = pav->tail->rlink = pav->tail->tail = NULL;
			pav->tail->size = pav->size;

			q = pav;
			p = pav->rlink;
		}
		else
		{
			p = (space)malloc(sizeof(word));
			q->rlink = p;
			p->tag = tag;
			p->location = location;
			p->size = size;
			p->llink = q;
			p->uplink = NULL;

			p->tail = (space)malloc(sizeof(word));
			p->tail->uplink = p;
			p->tail->location = p->location + p->size;
			p->tail->tag = p->tag;
			p->tail->llink = p->tail->rlink = p->tail->tail = NULL;
			p->tail->size = p->size;

			q = p;
			p = p->rlink;
		}

		counter++;
		ch = getchar();// '\n'
		ch = getchar();
	}
	pav->llink = q;
	q->rlink = pav;


	return pav;
}

void free_block(int location, int size)
{
	space p, q, r;
	p = pav;
	while (1)
	{//��λ
		if (location == 0)
			break;

		if (p->location < location && p->rlink->location >location)
			break;
		p = p->rlink;
	}
	q = p->rlink;

	if (location > p->tail->location && location + size < q->location)
	{//�����ڿ��Ϊռ�ÿ�
		r = (space)malloc(sizeof(word));
		r->location = location;
		r->size = size;
		r->tag = 0;
		r->llink = p;
		r->rlink = q;
		
		r->tail = (space)malloc(sizeof(word));
		r->tail->uplink = r;
		r->tail->location = r->location + r->size;
		r->tail->tag = r->tag;
		r->tail->llink = r->tail->rlink = r->tail->tail = NULL;
		r->tail->size = r->size;

		p->rlink = r;
		q->llink = r;
	}
	else if (location <= p->tail->location && location + size < q->location)
	{//���ڿ���ж����ڿ�Ϊռ��
		p->size += size;

		p->tail = (space)malloc(sizeof(word));
		p->tail->uplink = p;
		p->tail->location = p->location + p->size;
		p->tail->tag = p->tag;
		p->tail->llink = p->tail->rlink = p->tail->tail = NULL;
		p->tail->size = p->size;
	}
	else if (location > p->tail->location && location + size >= q->location)
	{
		q->location = location;
		q->size += size;

		q->tail->location = q->location + q->size;
		q->tail->size = q->size;
		
	}
	else
	{
		p->rlink = q->rlink;
		q->rlink->llink = p;

		p->size = p->size + q->size + size;

		p->tail = (space)malloc(sizeof(word));
		p->tail->uplink = p;
		p->tail->location = p->location + p->size;
		p->tail->tag = p->tag;
		p->tail->llink = p->tail->rlink = p->tail->tail = NULL;
		p->tail->size = p->size;

		if (p != q)
			free(q);
	}

	return;
}

space free_pav()
{
	int tag, location, size;
	int ch;
	ch = getchar();
	while (ch == '0'|| ch == '1')
	{
		scanf(" %d %d", &location, &size);
		tag = ch - '0';
		free_block(location, size);
		ch = getchar();// '\n'
		ch = getchar();
		
	}

	return pav;
}

void print_pav()
{
	space p;
	p = pav;
	do
	{
		printf("%d %d %d", p->tag, p->location, p->size);
		if (p->rlink != pav)
			printf("\n");
		p = p->rlink;
	} while (p!=pav);
	return;
}

int main()
{
	
	
	create_pav();
	free_pav();
	print_pav();
	return 0;
}