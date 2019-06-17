#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10
#define link 0
#define thread 1

/*
读取两个数组，然后先读先序数组，找到根节点
然后在中序数组里面找到根节点
它的左边就是左子树，右边就是右子树
 */

typedef int elemtype;

typedef struct BiTNode
{
	elemtype data;
	struct BiTNode *lchild, *rchild, *parent;
	//int ltag, rtag;
}node, *tree;

int preorder_array[MAXLENGTH];
int inorder_array[MAXLENGTH];
int pre_len;
int in_len;



void create_array()
{
    int ch;
    int number;
    int index1, index2;
    index1 = index2 = 0;
    scanf("%d", &number);
    ch = getchar();
    while (ch != EOF && ch != '\n' && ch != ';')
    {
        preorder_array[index1++] = number;
        scanf("%d", &number);
        ch = getchar();
    }
    preorder_array[index1] = number;
    preorder_array[index1+1] = '\0';
    pre_len = index1 + 1;
    
    scanf("%d", &number);
    ch = getchar();
    while (ch != EOF && ch != '\n' && ch != ';')
    {
        inorder_array[index2++] = number;
        scanf("%d", &number);
        ch = getchar();
    }
    inorder_array[index2] = number;
    inorder_array[index2 +1] = '\0';
    in_len = index2 +1;
    if (pre_len != in_len)
        exit(1);
    

    return;
}

/*
可以考虑使用递归方式
参数为根节点的位置，以及需要处理的数组的范围
 */
void create_tree(tree root)
{
    int index1, index2;
    if (root == NULL)
    {
        root = (tree)malloc(sizeof(node));
        root->data = 0;
        root->lchild = root->lchild = root->parent = NULL;
    }
    tree p;
    for (index1 = 0; index1 < pre_len; index1++)
    {

    }

    return;
}


int main()
{
    tree root = NULL;
    create_array();

    return 0;
}