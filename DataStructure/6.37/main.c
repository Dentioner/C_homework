#include<stdio.h>
#include<stdlib.h>
/*
scanf碰到null的时候，number是0
因此可以用这个特征来区分null
此时getchar()读到的是n
这种遍历方法，索引值和完全二叉树的编号一样
可以用这个性质来判断某个结点是哪个节点的儿子

可以先将input变成一个数组，方便在造树的适合访问数据


注意题目要求用栈进行非递归遍历
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
                        
            //注意这个地方没有考虑最后一个输入为null的情况
            //如果最后一个输入为null，那么下一个字符就不是逗号而是回车
            ch = getchar();//,
            
            
            data_array[index] = -1;//用-1表示null        
        }
        else
        {
            data_array[index] = number;
        }
        

        //下一个
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
        
        data_array[index++] = -1;//用-1表示null        
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
    int first_index_of_next_floor = 4;//下一层第一个节点的编号值
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