#pragma warning(disable:4996);
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
#define OK 0
#define ERROR 1
#define LESS -1
#define GREATER 1

typedef int ElemType;
typedef int Status;
typedef struct 
{
	ElemType *elem;
	int length;
	int listsize;
}SQList;


void Union(List &la, List lb)
{
	int la_len, lb_len, i;
	ElemType e;
	InitList(la);
	la_len = ListLength(la);
	lb_len = ListLength(lb);
	for (i = 1; i <= lb_len; i++)
	{
		GetElem(Lb, i, e);
		if (!LocateElem(La, e, equal))
			ListInsert(la, ++la_len, e);
	}
}

void MergeList(List la, List lb, List &lc)
{
	int la_len, lb_len;
	ElemType ai, bj;
	int i = 1, j = 1, k = 0;
	InitList(lc);
	la_len = ListLength(la);
	lb_len = ListLength(lb);
	while ((i<=la_len)&&(j<=lb_len))
	{
		GetElem(la, i, &ai);
		GetElem(lb, j, &bj);
		if (ai <= bj)
		{
			ListInsert(lc, ++k, ai);
			++i;
		}
		else
		{
			ListInsert(lc, ++k, bj);
			++j;
		}
		while (i<=la_len)
		{
			GetElem(la, i++, ai);
			ListInsert(lc, ++k, ai);
		}
		while (j<=lb_len)
		{
			GetElem(lb, j++, bj);
			ListInsert(lc, ++k, bj);
		}
	}
}

Status InitList_sq(SQList *L)
{
	L->elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (!L->elem)
		exit(OVERFLOW);
	L->length = 0;
	L->listsize = LIST_INIT_SIZE;
	return OK;
}

Status ListInsert_sq(SQList *L, int i, ElemType e)
{
	if (i<1 || i>L->length + 1)
		return ERROR;
	if (L->length >= L->listsize)
	{
		ElemType *newbase = (ElemType *)realloc(L->elem, L->listsize + LISTINCREMENT);
		if (!newbase)
			return ERROR;
		L->elem = newbase;
		L->listsize += LISTINCREMENT;
	}
	
	ElemType *p;
	ElemType *q = &(L->elem[i - 1]);
	for (p = &(L->elem[L->length - 1]); p >= q; --p)
		*(p + 1) = *p;
	*q = e;
	L->length++;
	

	return OK;
}

Status ListDelete_sq(SQList *L, int i, ElemType *e)
{
	ElemType *p, *q;
	p = &(L->elem[i - 1]);
	*e = *p;
	q = L->elem[L->length - 1];
	for (++p; p <= q; ++p)
		*(p - 1) = *p;
	L->length--;
	return OK;
}

Status LocateElem_sq(SQList *L, ElemType e,
	Status(*compare)(ElemType, ElemType))
{
	int i;
	ElemType *p;
	i = 1;
	p = L->elem;
	while (i <= L->length && 
		(*compare)(*p,e)!=0)
	{
		p++;
		i++;
	}
	if (i <= L->length)
		return i;
	else
		return 0;
}

Status(*compare)(ElemType a, ElemType b)
{

	if (a < b)
		return LESS;
	if (a > b)
		return GREATER;
	return 0;
}

void MergeList_sq(SQList *la, SQList *lb, SQList *lc)
{
	ElemType *pa, *pb, *pc, *pa_last, *pb_last;
	pa = la->elem;
	pb = lb->elem;
	lc->listsize = lc->length = la->length + lb->length;
	pc = lc->elem = (ElemType *)malloc(lc->listsize * sizeof(ElemType));
	if (!lc->elem)
		exit(OVERFLOW);
	pa_last = la->elem[la->length - 1];
	pb_last = lb->elem[lb->length - 1];
	while (pa<=pa_last && pb <=pb_last)
	{
		if (*pa <= *pb)
			*pc++ = *pa++;
		else
			*pc++ = *pb++;
	}
	while (pa <= pa_last)
		*pc++ = *pa++;
	while (pb <= pb_last)
		*pc++ = *pb++;

}