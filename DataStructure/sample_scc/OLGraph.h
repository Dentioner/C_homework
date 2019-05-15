#ifndef OLGRAPH_H_INCLUDED
#define OLGRAPH_H_INCLUDED
#include <stdio.h>
#include <malloc.h>

#define MAX_VERTEX_NUM  30 //��󶥵���
typedef char ElemType;
typedef struct ArcBox {
    int  tailvex, headvex;//β����ͷ�����ͼ�е�λ��
    //InfoType    info;   // �뻡��ص���Ϣ ��Ȩֵ
    struct ArcBox *hlink,
                  *tlink; //�ֱ����ӻ�ͷ��ͬ�ͻ�β��ͬ�Ļ�
}ArcNode; //�����
typedef struct VexNode {
    ElemType  data;    // ������Ϣ
    ArcBox  *firstin,
            *firstout; //�ֱ�ָ��ö����һ���뻡�ͳ���
}VexNode;//������
typedef struct {
    int vexnum, arcnum;
    VexNode  xlist[MAX_VERTEX_NUM];
}OLGraph;

void CreateGraph(OLGraph *g);
int LocateVex(OLGraph *g,ElemType v);
void ConnectedComponent(OLGraph *g);

#endif // OLGRAPH_H_INCLUDED
