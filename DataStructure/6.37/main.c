#include<stdio.h>
#include<stdlib.h>
/*
scanf����null��ʱ��number��0
��˿������������������null
��ʱgetchar()��������n
���ֱ�������������ֵ����ȫ�������ı��һ��
����������������ж�ĳ��������ĸ��ڵ�Ķ���

�����Ƚ�input���һ�����飬�������������ʺϷ�������


ע����ĿҪ����ջ���зǵݹ����
*/
#define MAXLENGTH 50

typedef int elemtype ;

typedef struct BiTNode
{
    elemtype data;
    struct BiTNode *lchild, *rchild, *parent;
}node, *tree;


int data_array[MAXLENGTH];


void create_array()
{
    int ch, number;
    int index = 0;
    scanf("%d", &number);
    ch = getchar();
    while(ch!=EOF && ch!= '\n' && ch != '\0')
    {
        
        if (number == 0)//null
        {
            ch = getchar();//n
            ch = getchar();//u
            ch = getchar();//l
            ch = getchar();//l
                        
            //ע������ط�û�п������һ������Ϊnull�����
            //������һ������Ϊnull����ô��һ���ַ��Ͳ��Ƕ��Ŷ��ǻس�
            ch = getchar();//,
            
            
            data_array[index] = -1;//��-1��ʾnull        
        }
        else
        {
            data_array[index] = number;
        }
        

        //��һ��
            scanf("%d", &number);
            ch = getchar();
            index ++;
    }


    if (number == 0)//null
    {
        ch = getchar();//n
        ch = getchar();//u
        ch = getchar();//l
        ch = getchar();//l
        
        data_array[index++] = -1;//��-1��ʾnull        
    }
    else
    {
        data_array[index++] = number;
    }

    data_array[index] = '\0';

    return;


}


void create_tree()
{
    int index = 2;
    int first_index_of_next_floor = 4;//��һ���һ���ڵ�ı��ֵ
    int ch, number;
    tree root, p;
    root = (tree)malloc(sizeof(node));

    scanf("%d", &number);
    ch = getchar();
    root->data = number;
    root->parent = NULL;
    root->lchild = root->rchild = NULL;

    

}

int main()
{
    create_array();

    system("pause");
    return 0;
}