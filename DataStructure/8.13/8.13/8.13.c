#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
//#define FootLoc(p) (p+p->size-1)
#define MEMSIZE 10000


typedef struct WORD
{
	union 
	{
		struct WORD *llink;//ͷ����ָ����б��ǰ����㣬�����head�õ�
		struct WORD *uplink;//β����ָ�򱾽���ͷ���������foot�õ�
	};
	int tag;//0�����У�1��ռ��
	int location;//��ʼ��ַ
	int size;
	struct WORD *rlink;//ͷ����,ָ���̽��

}word, head, foot, *space;


const int e = 16; //������С�ڵ���e��ʣ����

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

space create_pav(space pav)
{
	head *p, *p2;
	//foot *q;
	int ch;
	int tag, location, size;
	scanf("%d %d %d", &tag, &location, &size);
	ch = getchar();// '\n'
	


	pav = (space)malloc(size);
	//q = (space)malloc(sizeof(word));
	//pav->llink = pav->rlink = pav->uplink = NULL;
	pav->rlink = NULL;
	pav->tag = tag;
	pav->size = size;
	pav->location = location;
	pav->llink = NULL;//��һ���ڵ�û��ǰ��
	

	p2 = NULL;
	p = pav;
	//q = (space)(p + p->size);
	//q->uplink = p;
	//q->tag = p->tag;
	//q->location = p->location + p->size;
	p2 = p;
	p = p->rlink;

	ch = getchar();//��һ���ַ�������ǻ��еĻ���˵��Ҫ��ӵĽڵ㶼�������
	while (ch != '\n')
	{
		tag = ch - '0';//����ղ�chû�ж������з������Ͱ�tag������
		scanf(" %d %d", &location, &size);
		ch = getchar();// '\n'
		p = (space)malloc(size);
		
		p->tag = tag;
		p->location = location;
		p->size = size;
		//q = (space)(p + p->size);
		//q->uplink = p;
		//q->tag = p->tag;
		//q->location = p->location + p->size;
		p->llink = p2;
		p2->rlink = p;
		
		p2 = p;
		p = p->rlink;

		
		ch = getchar();//��һ���ַ�������ǻ��еĻ���˵��Ҫ��ӵĽڵ㶼�������

		
	}

	//�������з������һ������
	/*
		p->llink = p2;
		p2->rlink = p;
	
	p->tag = tag;
	p->location = location;
	p->size = size;
*/
	//q->uplink = p;
	//q->tag = p->tag;
	//q->location = p->location + p->size;

	p2->rlink = pav;//ָ��ȥ
	pav->llink = p2;


	return pav;
}

void free_pav(space *pav)
{
	int tag, location, size;
	int ch;
	scanf("%d %d %d", &tag, &location, &size);
	ch = getchar();// '\n'
	FreeBoundTag(pav, location, size);
	
	
	ch = getchar();
	while (ch == '0' || ch == '1')
	{
		tag = ch - '0';
		scanf(" %d %d", &location, &size);

		FreeBoundTag(pav, location, size);
		ch = getchar();// '\n'
		if (ch == '\n')
			ch = getchar();
	}
	return;
}

int main()
{
	space pav = NULL;
	pav = create_pav(pav);
	free_pav(&pav);
	return 0;
}