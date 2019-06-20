#include <iostream>
#include <malloc.h>
#include <stdio.h>
using namespace std;

#define MEMSIZE 10000
const int e = 16; //不保留小于等于e的剩余量
typedef  struct word {
	union {
		//头部域，指向空闲表的前驱结点
		struct word *llink;
		//尾部域，指向本结点的头部
		struct word *uplink;
	};
	int tag; //0：空闲；1：占用
	int size;
	struct word  *rlink;//头部域,指向空闲表的后继结点
}WORD, head, foot, *Space;

//#define FootLoc(p) (p + p->size -1)//指向p所指结点的底部，若分配单位为WORD
Space FootLoc(Space p) { //分配单位为 字符/字节
	char *q; Space f;
	q = (char *)p;
	f = (Space)(q + sizeof(char)*p->size - sizeof(foot));
	return f;
}

//若有不小于n的空闲块，则分配相应的存储块，并返回其首地址，否则返回NULL
char* AllocBoundTag(Space pav, int n) {
	Space p, f;
	n += sizeof(WORD) * 2;
	// 查找不小于n的空闲块
	for (p = pav; p && p->size < n && p->rlink != pav; p = p->rlink);
	if (!p || p->size < n) return NULL; //找不到，返回空指针
	else { //p指向找到的空闲块
		f = FootLoc(p); //f指向底部
		pav = p->rlink; //pav指向*p结点的后继结点
		if (p->size - n <= e) { //整块分配，不保留<=e的剩余量
			if (pav == p)
				pav = NULL; // 可利用空间表变为空表
			else { //在表中删除分配的结点
				pav->llink = p->llink;
				p->llink->rlink = pav;
			}
			p->tag = f->tag = 1; // 修改分配结点的头部和底部标志
		}
		else { //分配该块的后n个字
			f->tag = 1; 	// 修改分配块的底部标志
			p->size -= n; 	// 置剩余块大小
			f = FootLoc(p); // 指向剩余块底部
			f->tag = 0; f->uplink = p; // 设置剩余块底部
			p = f + 1;	 // 指向分配块头部
			p->tag = 1; p->size = n; // 设置分配块头部
			//p->llink=p->rlink=NULL;
		}
		return (char*)(p + 1); // 返回分配块首地址
	}
} // AllocBoundTag

void FreeBoundTag(Space pav, char* x) {
	struct word *p, *q, *f, *s, *t, *q1; int n;

	p = (struct word *)(x - sizeof(WORD));
	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 1) {//左、右邻块均为占用块，OK
		p->tag = 0;
		f = FootLoc(p);
		f->uplink = p;
		f->tag = 0;
		if (!pav)
			pav = p->llink = p->rlink = p; //第一个空闲块
		else {
			q = pav->llink;  p->rlink = pav;
			p->llink = q;
			q->rlink = p;
			pav->llink = p;
			pav = p;
			//pav指向刚释放的空闲结点
			//刚释放的空闲结点是插在pav之前，并成为pav
			//下次分配最先查询的结点
		}
		return;
	}

	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 1) {//左邻块空闲而右邻块为占用,OK
		n = p->size;
		s = (p - 1)->uplink; s->size += n; //设置新空闲块大小
		f = FootLoc(p); //f=p+n-1;
		f->uplink = s; f->tag = 0; //设置新空闲块底部
		return;
	}

	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 0) {//左邻占用而右邻空闲,OK
		t = FootLoc(p) + 1; //t=p+p->size ;
		p->tag = 0; q = t->llink;
		p->llink = q; q->rlink = p; // 设置空闲块的前驱
		q1 = t->rlink; p->rlink = q1; q1->llink = p;
		p->size += t->size; //设置新空闲块大小
		FootLoc(t)->uplink = p; //底部指针指向新结点的头部
		return;
	}
	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 0) { //左、右邻块均为空闲块，OK
		n = p->size;
		s = (p - 1)->uplink;	//s指向左邻块
		t = FootLoc(p) + 1;     //t=p+p->size; 	//t指向右邻块
		s->size += n + t->size; //设置新结点的大小
		q = t->llink; q1 = t->rlink; //修改右邻原来的关系
		q->rlink = q1; q1->llink = q;
		FootLoc(t)->uplink = s; //新结点底部指针指向其头部
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
	s1=AllocBoundTag(pav, 100); //分配100字节
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
