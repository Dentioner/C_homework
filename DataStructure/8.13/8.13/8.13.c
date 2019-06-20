#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
//#define FootLoc(p) (p+p->size-1)



typedef struct WORD
{
	
	struct WORD *llink;//头部域，指向空闲表的前驱结点，这个是head用的
	struct WORD *uplink;//尾部域，指向本结点的头部，这个是foot用的
	struct WORD *tail;
	int tag;//0：空闲；1：占用
	int location;//起始地址
	int size;
	struct WORD *rlink;//头部域,指向后继结点

}word, head, foot, *space;


const int e = 16; //不保留小于等于e的剩余量
space pav = NULL;

space FootLoc(space p) 
{ //分配单位为 字符/字节
	char *q; 
	space f;
	q = (char *)p;
	f = (space)(q + sizeof(char)*p->size - sizeof(foot));
	return f;
}

space AllocBoundTag(space *pav, int n)
{//若有不小于n的空闲块，则分配相应的存储块，并返回其首地址，否则返回NULL
	//pav就是空闲表
	space p, f;
	for (p = (*pav); p && p->size < n && p->rlink != (*pav); p = p->rlink)
		;//查找不小于n的空闲块
	if (!p || p->size < n)
		return NULL;//找不到就返回NULL
	else
	{//此时p就是找到的空闲块
		f = FootLoc(p);//指向p的底部
		(*pav) = p->rlink;//pav指向p的下家
		if (p->size - n <= e)
		{//剩下来的大小不够阈值了，全部白给
			if ((*pav) == p)
				pav = NULL;//可利用空间表变成空表了
			else
			{
				(*pav)->llink = p->llink;
				p->llink->rlink = (*pav);
			}
			p->tag = f->tag = 1;//修改分配节点的头部和底部标志
		}
		else//给出这坨空闲区域的n个字
		{
			f->tag = 1;//修改底部标志
			p->size -= n;//大小减去n
			f = FootLoc(p);//指向剩余块的底部
			f->tag = 0;
			f->uplink = p;
			p = f + 1;//指向被分配的块的头部
			p->tag = 1;//标记改成1，被使用了
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
	{//左、右邻块均为占用块，此时只要作简单的插入即可
		p->tag = 0;
		f = FootLoc(p);
		f->uplink = p;
		f->tag = 0;
		if (!(*pav))
			(*pav) = p->llink = p->rlink = p; //第一个空闲块
		else 
		{
			q = (*pav)->llink;  p->rlink = (*pav);
			p->llink = q;
			q->rlink = p;
			(*pav)->llink = p;
			(*pav) = p;
			//pav指向刚释放的空闲结点
			//刚释放的空闲结点是插在pav之前，并成为pav
			//下次分配最先查询的结点
		}
		return;
	}

	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 1) 
	{//左邻块空闲而右邻块为占用, 只要改变左邻空闲块的节点，也就是和左边合体
		n = p->size;
		s = (p - 1)->uplink; 
		s->size += n; //设置新空闲块大小
		f = FootLoc(p); 
		//f=p+n-1;
		f->uplink = s; 
		f->tag = 0; //设置新空闲块底部
		return;
	}

	if ((p - 1)->tag == 1 && (FootLoc(p) + 1)->tag == 0) 
	{//左邻占用而右邻空闲, 处理方法同上
		t = FootLoc(p) + 1; 
		//t = p + p->size;
		p->tag = 0; 
		q = t->llink;
		p->llink = q; 
		q->rlink = p; // 设置空闲块的前驱
		q1 = t->rlink; 
		p->rlink = q1; 
		q1->llink = p;
		p->size += t->size; //设置新空闲块大小
		FootLoc(t)->uplink = p; //底部指针指向新结点的头部
		return;
	}
	if ((p - 1)->tag == 0 && (FootLoc(p) + 1)->tag == 0) 
	{ //左、右邻块均为空闲块，一起合体
		n = p->size;
		s = (p - 1)->uplink;	//s指向左邻块
		t = FootLoc(p) + 1;     //t=p+p->size; 	//t指向右邻块
		s->size += n + t->size; //设置新结点的大小
		q = t->llink; 
		q1 = t->rlink; //修改右邻原来的关系
		q->rlink = q1; 
		q1->llink = q;
		FootLoc(t)->uplink = s; //新结点底部指针指向其头部
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
	{//定位
		if (location == 0)
			break;

		if (p->location < location && p->rlink->location >location)
			break;
		p = p->rlink;
	}
	q = p->rlink;

	if (location > p->tail->location && location + size < q->location)
	{//左、右邻块均为占用块
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
	{//左邻块空闲而右邻块为占用
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