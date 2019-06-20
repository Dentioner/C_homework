#include <iostream>
#include <malloc.h>
#include <stdio.h>
using namespace std;

#define MEMSIZE 10000
const int e = 16; //������С�ڵ���e��ʣ����
typedef  struct word {
	union {
		//ͷ����ָ����б��ǰ�����
		struct word *llink;
		//β����ָ�򱾽���ͷ��
		struct word *uplink;
	};
	int tag; //0�����У�1��ռ��
	int size;
	struct word  *rlink;//ͷ����,ָ����б�ĺ�̽��
}WORD, head, foot, *Space;

//#define FootLoc(p) (p + p->size -1)//ָ��p��ָ���ĵײ��������䵥λΪWORD
Space FootLoc(Space p) { //���䵥λΪ �ַ�/�ֽ�
	char *q; Space f;
	q = (char *)p;
	f = (Space)(q + sizeof(char)*p->size - sizeof(foot));
	return f;
}

//���в�С��n�Ŀ��п飬�������Ӧ�Ĵ洢�飬���������׵�ַ�����򷵻�NULL
char* AllocBoundTag(Space pav, int n) {
	Space p, f;
	n += sizeof(WORD) * 2;
	// ���Ҳ�С��n�Ŀ��п�
	for (p = pav; p && p->size < n && p->rlink != pav; p = p->rlink);
	if (!p || p->size < n) return NULL; //�Ҳ��������ؿ�ָ��
	else { //pָ���ҵ��Ŀ��п�
		f = FootLoc(p); //fָ��ײ�
		pav = p->rlink; //pavָ��*p���ĺ�̽��
		if (p->size - n <= e) { //������䣬������<=e��ʣ����
			if (pav == p)
				pav = NULL; // �����ÿռ���Ϊ�ձ�
			else { //�ڱ���ɾ������Ľ��
				pav->llink = p->llink;
				p->llink->rlink = pav;
			}
			p->tag = f->tag = 1; // �޸ķ������ͷ���͵ײ���־
		}
		else { //����ÿ�ĺ�n����
			f->tag = 1; 	// �޸ķ����ĵײ���־
			p->size -= n; 	// ��ʣ����С
			f = FootLoc(p); // ָ��ʣ���ײ�
			f->tag = 0; f->uplink = p; // ����ʣ���ײ�
			p = f + 1;	 // ָ������ͷ��
			p->tag = 1; p->size = n; // ���÷����ͷ��
			//p->llink=p->rlink=NULL;
		}
		return (char*)(p + 1); // ���ط�����׵�ַ
	}
} // AllocBoundTag

void FreeBoundTag(Space pav, char* x) {
	struct word *p, *q, *f, *s, *t, *q1; int n;

	p = (struct word *)(x - sizeof(WORD));
	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 1) {//�����ڿ��Ϊռ�ÿ飬OK
		p->tag = 0;
		f = FootLoc(p);
		f->uplink = p;
		f->tag = 0;
		if (!pav)
			pav = p->llink = p->rlink = p; //��һ�����п�
		else {
			q = pav->llink;  p->rlink = pav;
			p->llink = q;
			q->rlink = p;
			pav->llink = p;
			pav = p;
			//pavָ����ͷŵĿ��н��
			//���ͷŵĿ��н���ǲ���pav֮ǰ������Ϊpav
			//�´η������Ȳ�ѯ�Ľ��
		}
		return;
	}

	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 1) {//���ڿ���ж����ڿ�Ϊռ��,OK
		n = p->size;
		s = (p - 1)->uplink; s->size += n; //�����¿��п��С
		f = FootLoc(p); //f=p+n-1;
		f->uplink = s; f->tag = 0; //�����¿��п�ײ�
		return;
	}

	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 0) {//����ռ�ö����ڿ���,OK
		t = FootLoc(p) + 1; //t=p+p->size ;
		p->tag = 0; q = t->llink;
		p->llink = q; q->rlink = p; // ���ÿ��п��ǰ��
		q1 = t->rlink; p->rlink = q1; q1->llink = p;
		p->size += t->size; //�����¿��п��С
		FootLoc(t)->uplink = p; //�ײ�ָ��ָ���½���ͷ��
		return;
	}
	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 0) { //�����ڿ��Ϊ���п飬OK
		n = p->size;
		s = (p - 1)->uplink;	//sָ�����ڿ�
		t = FootLoc(p) + 1;     //t=p+p->size; 	//tָ�����ڿ�
		s->size += n + t->size; //�����½��Ĵ�С
		q = t->llink; q1 = t->rlink; //�޸�����ԭ���Ĺ�ϵ
		q->rlink = q1; q1->llink = q;
		FootLoc(t)->uplink = s; //�½��ײ�ָ��ָ����ͷ��
		return;
	}
}

void PrintMemBoundTag(Space pav) {
	Space p; int total; char *q;
	p = pav; total = 0; printf("Print:\n");
	while (total < MEMSIZE) {
		printf("%d %d\n", p->tag, p->size);
		total += p->size;
		q = (char *)p; q = q + p->size;
		p = (Space)q;
	}
}
int main()
{
	char *p; p = (char *)malloc(sizeof(char)*MEMSIZE);
	Space pav;
	pav = (Space)p;
	pav->llink = pav->rlink = pav;
	pav->tag = 0; pav->size = MEMSIZE;

	char *s1, *s2, *s3;
	/*
	s1=AllocBoundTag(pav, 100); //����100�ֽ�
	s2=AllocBoundTag(pav, 150);
	s3=AllocBoundTag(pav, 200);
	PrintMemBoundTag(pav);// Print: 0 9454 1 232 1 182 1 132
	FreeBoundTag(pav, s2);
	PrintMemBoundTag(pav);// Print: 0 9454 1 232 0 182 1 132
	FreeBoundTag(pav, s3);
	PrintMemBoundTag(pav);// Print: 0 9868 1 132
	s1=AllocBoundTag(pav, 300);
	PrintMemBoundTag(pav);// Print: 0 9536 1 332 1 132
	*/

	s1 = AllocBoundTag(pav, 100);
	s2 = AllocBoundTag(pav, 150);
	s3 = AllocBoundTag(pav, 200);
	PrintMemBoundTag(pav);//Print: 0 9454 1 232 1 182 1 132
	s1 = AllocBoundTag(pav, 300);
	PrintMemBoundTag(pav);//Print: 0 9122 1 332 1 232 1 182 1 132
	FreeBoundTag(pav, s2);
	FreeBoundTag(pav, s3);
	PrintMemBoundTag(pav);//Print: 0 9122 1 332 0 414 1 132

	system("pause");
	return 0;
}
